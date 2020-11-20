// Copyright (c) 2020 Intel Corporation
//
// SPDX-License-Identifier: BSD-2-Clause-Patent

#![allow(unused)]

#![feature(llvm_asm)]
#![cfg_attr(not(test), no_std)]
#![cfg_attr(not(test), no_main)]
#![cfg_attr(test, allow(unused_imports))]

#[macro_use]
mod logger;
mod pcd;
mod pi;
mod sec;

extern crate plain;
mod elf;

use r_efi::efi;

use crate::pi::hob;

use core::panic::PanicInfo;

use core::ffi::c_void;

#[allow(non_snake_case)]
#[repr(C)]
#[derive(Copy, Clone, Debug)]
pub struct HobTemplate {
    pub handoffInfoTable: hob::HandoffInfoTable,
    pub firmwareVolume: hob::FirmwareVolume,
    pub cpu: hob::Cpu,
    pub hypervisorFw: hob::MemoryAllocation,
    pub pageTable: hob::MemoryAllocation,
    pub stack: hob::MemoryAllocation,
    pub memoryAbove1M: hob::ResourceDescription,
    pub memoryBlow1M: hob::ResourceDescription,
    pub endOffHob:  hob::Header
}

#[cfg(not(test))]
#[panic_handler]
#[allow(clippy::empty_loop)]
fn panic(_info: &PanicInfo) -> ! {
    log!("panic ... {:?}\n", _info);
    //log!("panic...");
    loop {}
}

#[cfg(not(test))]
#[no_mangle]
#[export_name = "SecCoreStartupWithStack"]
pub extern "win64" fn _start(boot_fv: *const c_void, top_of_stack: *const c_void) -> ! {
    log!("Starting SEC boot_fv - {:p}, Top of stack - {:p} \n", boot_fv, top_of_stack);

    log!(" EfiTop: {:p}\n",((top_of_stack as u64) - (pcd::pcd_get_PcdOvmfSecPeiTempRamSize() as u64)) as *const c_void);

    log!(" PcdOvmfDxeMemFvBase: 0x{:X}\n", pcd::pcd_get_PcdOvmfDxeMemFvBase());
    log!(" PcdOvmfDxeMemFvSize: 0x{:X}\n", pcd::pcd_get_PcdOvmfDxeMemFvSize());
    log!(" PcdOvmfPeiMemFvBase: 0x{:X}\n", pcd::pcd_get_PcdOvmfPeiMemFvBase());
    log!(" PcdOvmfPeiMemFvSize: 0x{:X}\n", pcd::pcd_get_PcdOvmfPeiMemFvSize());
    log!(" PcdOvmfSecPageTablesBase: 0x{:X}\n", pcd::pcd_get_PcdOvmfSecPageTablesBase());
    log!(" PcdOvmfSecPeiTempRamBase: 0x{:X}\n", pcd::pcd_get_PcdOvmfSecPeiTempRamBase());
    log!(" PcdOvmfSecPeiTempRamSize: 0x{:X}\n", pcd::pcd_get_PcdOvmfSecPeiTempRamSize());

    let mut hob_header = hob::Header {
        r#type: hob::HOB_TYPE_END_OF_HOB_LIST,
        length: core::mem::size_of::<hob::Header>() as u16,
        reserved: 0
    };

    let (memory_top, memory_bottom) = sec::GetSystemMemorySizeBelow4Gb();
    #[allow(non_snake_case)]
    let mut handoffInfoTable = hob::HandoffInfoTable {
        header: hob::Header {
            r#type: hob::HOB_TYPE_HANDOFF,
            length: core::mem::size_of::<hob::HandoffInfoTable>() as u16,
            reserved: 0
        },
        version: 9u32,
        boot_mode: pi::boot_mode::BOOT_WITH_FULL_CONFIGURATION,
        efi_memory_top: memory_top,
        efi_memory_bottom: memory_bottom,
        efi_free_memory_top: memory_top,
        efi_free_memory_bottom: memory_bottom + sec::EfiPageToSize(sec::EfiSizeToPage(core::mem::size_of::<HobTemplate>() as u64)),
        efi_end_of_hob_list: memory_bottom + core::mem::size_of::<HobTemplate>() as u64
    };

    #[allow(non_snake_case)]
    let mut firmwareVolume = hob::FirmwareVolume {
        header: hob::Header {
            r#type: hob::HOB_TYPE_FV,
            length: core::mem::size_of::<hob::FirmwareVolume>() as u16,
            reserved: 0
        },
        base_address: boot_fv as u64 - pcd::pcd_get_PcdOvmfDxeMemFvBase() as u64,
        length: pcd::pcd_get_PcdOvmfDxeMemFvSize() as u64,
    };

    #[allow(non_snake_case)]
    let mut cpu = hob::Cpu {
        header: hob::Header {
            r#type: hob::HOB_TYPE_CPU,
            length: core::mem::size_of::<hob::Cpu>() as u16,
            reserved: 0
        },
        size_of_memory_space: 36u8, // TBD asmcpuid
        size_of_io_space: 16u8,
        reserved: [0u8; 6]
    };

    const MEMORY_ALLOCATION_STACK_GUID: efi::Guid = efi::Guid::from_fields(
        0x4ED4BF27, 0x4092, 0x42E9, 0x80, 0x7D, &[0x52, 0x7B, 0x1D, 0x00, 0xC9, 0xBD]
    );
    let mut stack = hob::MemoryAllocation {
        header: hob::Header {
            r#type: hob::HOB_TYPE_MEMORY_ALLOCATION,
            length: core::mem::size_of::<hob::MemoryAllocation>() as u16,
            reserved: 0
        },
        alloc_descriptor: hob::MemoryAllocationHeader {
            name: MEMORY_ALLOCATION_STACK_GUID,
            memory_base_address: pcd::pcd_get_PcdOvmfSecPeiTempRamBase() as u64,
            memory_length: pcd::pcd_get_PcdOvmfSecPeiTempRamSize() as u64,
            memory_type: efi::MemoryType::BootServicesData,
            reserved: [0u8; 4]
        }
    };

    // Enable host Paging //TBD
    const PAGE_TABLE_NAME_GUID: efi::Guid = efi::Guid::from_fields(
        0xF8E21975, 0x0899, 0x4F58, 0xA4, 0xBE, &[0x55, 0x25, 0xA9, 0xC6, 0xD7, 0x7A]
    );

    #[allow(non_snake_case)]
    let mut pageTable = hob::MemoryAllocation {
        header: hob::Header {
            r#type: hob::HOB_TYPE_MEMORY_ALLOCATION,
            length: core::mem::size_of::<hob::MemoryAllocation>() as u16,
            reserved: 0
        },
        alloc_descriptor: hob::MemoryAllocationHeader {
            name: PAGE_TABLE_NAME_GUID,
            memory_base_address: 0u64,  // TBD
            memory_length: 0u64,        // TBD
            memory_type: efi::MemoryType::BootServicesData,
            reserved: [0u8; 4]
        }
    };

    const DEFAULT_GUID: efi::Guid = efi::Guid::from_fields(
        0x4ED4BF27, 0x4092, 0x42E9, 0x80, 0x7D, &[0x52, 0x7B, 0x1D, 0x00, 0xC9, 0xBD]
    );
    #[allow(non_snake_case)]
    let mut memoryAbove1M = hob::ResourceDescription {
        header: hob::Header {
            r#type: hob::HOB_TYPE_RESOURCE_DESCRIPTOR,
            length: core::mem::size_of::<hob::ResourceDescription>() as u16,
            reserved: 0
        },
        owner: efi::Guid::from_fields(
            0x4ED4BF27, 0x4092, 0x42E9, 0x80, 0x7D, &[0x52, 0x7B, 0x1D, 0x00, 0xC9, 0xBD]
        ),
        resource_type: hob::RESOURCE_SYSTEM_MEMORY,
        resource_attribute: hob::RESOURCE_ATTRIBUTE_PRESENT |
            hob::RESOURCE_ATTRIBUTE_INITIALIZED |
            hob::RESOURCE_ATTRIBUTE_UNCACHEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
            hob::RESOURCE_ATTRIBUTE_TESTED,
        physical_start: 0x100000u64,  // TBD
        resource_length: 0u64   // TBD
    };

    #[allow(non_snake_case)]
    let mut memoryBelow1M = hob::ResourceDescription {
        header: hob::Header {
            r#type: hob::HOB_TYPE_RESOURCE_DESCRIPTOR,
            length: core::mem::size_of::<hob::ResourceDescription>() as u16,
            reserved: 0
        },
        owner: efi::Guid::from_fields(
            0x4ED4BF27, 0x4092, 0x42E9, 0x80, 0x7D, &[0x52, 0x7B, 0x1D, 0x00, 0xC9, 0xBD]
        ),
        resource_type: hob::RESOURCE_SYSTEM_MEMORY,
        resource_attribute: hob::RESOURCE_ATTRIBUTE_PRESENT |
            hob::RESOURCE_ATTRIBUTE_INITIALIZED |
            hob::RESOURCE_ATTRIBUTE_UNCACHEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
            hob::RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
            hob::RESOURCE_ATTRIBUTE_TESTED,
        physical_start: 0u64,  // TBD
        resource_length: 0x8000u64 + 0x2000u64  // TBD
    };

    #[allow(non_snake_case)]
    let mut hypervisorFw = hob::MemoryAllocation {
        header: hob::Header {
            r#type: hob::HOB_TYPE_MEMORY_ALLOCATION,
            length: core::mem::size_of::<hob::MemoryAllocation>() as u16,
            reserved: 0
        },
        alloc_descriptor: hob::MemoryAllocationHeader {
            name: HYPERVISORFW_NAME_GUID,
            memory_base_address: 0u64,  // TBD
            memory_length: 0u64,        // TBD
            memory_type: efi::MemoryType::BootServicesCode,
            reserved: [0u8; 4]
        }
    };

    const HYPERVISORFW_NAME_GUID: efi::Guid = efi::Guid::from_fields(
        0x6948d4a, 0xd359, 0x4721, 0xad, 0xf6, &[0x52, 0x25, 0x48, 0x5a, 0x6a, 0x3a]
    );
    let mut hob_template = HobTemplate {
        handoffInfoTable: handoffInfoTable,
        firmwareVolume: firmwareVolume,
        cpu: cpu,
        hypervisorFw: hypervisorFw,
        pageTable: pageTable,
        stack: stack,
        memoryAbove1M: memoryAbove1M,
        memoryBlow1M: memoryBelow1M,
        endOffHob:  hob::Header {
            r#type: hob::HOB_TYPE_END_OF_HOB_LIST,
            length: core::mem::size_of::<hob::Header>() as u16,
            reserved: 0
        }
    };
    log!( " Hob prepare\n");

    let mut entry: usize = 0usize;
    entry = sec::FindAndReportEntryPoint(pcd::pcd_get_PcdOvmfDxeMemFvBase() as u64 as * const pi::fv::FirmwareVolumeHeader) as usize;
    let mut code: extern "win64" fn(* mut HobTemplate) = unsafe {core::mem::transmute(entry)};
    code(&hob_template as *const HobTemplate as *mut HobTemplate);

    loop {}
}
