/** @file
  Library instance that implements TimerLib library class based on PPIs
  gEdkiiPeiStall2PpiGuid and gEdkiiPeiTimestampPpiGuid.

  Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php.

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/


#include <PiPei.h>

#include <Ppi/Stall2.h>
#include <Ppi/Timestamp.h>

#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PeiServicesLib.h>

/**
  Get Stall2 PPI.

  @return Stall2 PPI.

**/
EDKII_PEI_STALL2_PPI *
GetStall2Ppi (
  VOID
  )
{
  EFI_STATUS            Status;
  EDKII_PEI_STALL2_PPI  *Stall2Ppi;

  Status = PeiServicesLocatePpi (
             &gEdkiiPeiStall2PpiGuid,
             0,
             NULL,
             (VOID **) &Stall2Ppi
             );
  ASSERT_EFI_ERROR (Status);
  ASSERT (Stall2Ppi != NULL);

  return Stall2Ppi;
}

/**
  Get Timestamp PPI.

  @return Timestamp PPI.

**/
EDKII_PEI_TIMESTAMP_PPI *
GetTimestampPpi (
  VOID
  )
{
  EFI_STATUS                Status;
  EDKII_PEI_TIMESTAMP_PPI   *TimestampPpi;

  Status = PeiServicesLocatePpi (
             &gEdkiiPeiTimestampPpiGuid,
             0,
             NULL,
             (VOID **) &TimestampPpi
             );

  ASSERT_EFI_ERROR (Status);
  ASSERT (TimestampPpi != NULL);

  return TimestampPpi;
}

/**
  Stalls the CPU for at least the given number of microseconds.

  Stalls the CPU for the number of microseconds specified by MicroSeconds.

  @param  MicroSeconds  The minimum number of microseconds to delay.

  @return The value of MicroSeconds inputted.

**/
UINTN
EFIAPI
MicroSecondDelay (
  IN      UINTN                     MicroSeconds
  )
{
  EDKII_PEI_STALL2_PPI  *Stall2Ppi;

  Stall2Ppi = GetStall2Ppi ();

  //
  // The unit of Stall2 is nonosecond.
  //
  Stall2Ppi->Stall2 (Stall2Ppi, MicroSeconds * 1000);

  return MicroSeconds;
}

/**
  Stalls the CPU for at least the given number of nanoseconds.

  Stalls the CPU for the number of nanoseconds specified by NanoSeconds.

  @param  NanoSeconds The minimum number of nanoseconds to delay.

  @return The value of NanoSeconds inputted.

**/
UINTN
EFIAPI
NanoSecondDelay (
  IN      UINTN                     NanoSeconds
  )
{
  EDKII_PEI_STALL2_PPI  *Stall2Ppi;

  Stall2Ppi = GetStall2Ppi ();

  //
  // The unit of Stall2 is nonosecond.
  //
  Stall2Ppi->Stall2 (Stall2Ppi, NanoSeconds);

  return NanoSeconds;
}

/**
  Retrieves the current value of a 64-bit free running performance counter.

  The counter can either count up by 1 or count down by 1. If the physical
  performance counter counts by a larger increment, then the counter values
  must be translated. The properties of the counter can be retrieved from
  GetPerformanceCounterProperties().

  @return The current value of the free running performance counter.

**/
UINT64
EFIAPI
GetPerformanceCounter (
  VOID
  )
{
  EDKII_PEI_TIMESTAMP_PPI   *TimestampPpi;

  TimestampPpi = GetTimestampPpi ();

  return TimestampPpi->GetTimestamp (TimestampPpi);
}

/**
  Retrieves the 64-bit frequency in Hz and the range of performance counter
  values.

  If StartValue is not NULL, then the value that the performance counter starts
  with immediately after is it rolls over is returned in StartValue. If
  EndValue is not NULL, then the value that the performance counter end with
  immediately before it rolls over is returned in EndValue. The 64-bit
  frequency of the performance counter in Hz is always returned. If StartValue
  is less than EndValue, then the performance counter counts up. If StartValue
  is greater than EndValue, then the performance counter counts down. For
  example, a 64-bit free running counter that counts up would have a StartValue
  of 0 and an EndValue of 0xFFFFFFFFFFFFFFFF. A 24-bit free running counter
  that counts down would have a StartValue of 0xFFFFFF and an EndValue of 0.

  @param  StartValue  The value the performance counter starts with when it
                      rolls over.
  @param  EndValue    The value that the performance counter ends with before
                      it rolls over.

  @return The frequency in Hz.

**/
UINT64
EFIAPI
GetPerformanceCounterProperties (
  OUT      UINT64                    *StartValue,  OPTIONAL
  OUT      UINT64                    *EndValue     OPTIONAL
  )
{
  EFI_STATUS                Status;
  EDKII_PEI_TIMESTAMP_PPI   *TimestampPpi;
  EFI_TIMESTAMP_PROPERTIES  Properties;

  TimestampPpi = GetTimestampPpi ();

  Status = TimestampPpi->GetProperties (TimestampPpi, &Properties);
  ASSERT_EFI_ERROR (Status);

  if (StartValue != NULL) {
    *StartValue = 0;
  }

  if (EndValue != NULL) {
    *EndValue = Properties.EndValue;
  }

  return Properties.Frequency;
}

/**
  Converts elapsed ticks of performance counter to time in nanoseconds.

  This function converts the elapsed ticks of running performance counter to
  time value in unit of nanoseconds.

  @param  Ticks     The number of elapsed ticks of running performance counter.

  @return The elapsed time in nanoseconds.

**/
UINT64
EFIAPI
GetTimeInNanoSecond (
  IN      UINT64                     Ticks
  )
{
  UINT64  Frequency;
  UINT64  NanoSeconds;
  UINT64  Remainder;
  INTN    Shift;

  Frequency = GetPerformanceCounterProperties (NULL, NULL);

  //
  //          Ticks
  // Time = --------- x 1,000,000,000
  //        Frequency
  //
  NanoSeconds = MultU64x32 (DivU64x64Remainder (Ticks, Frequency, &Remainder), 1000000000u);

  //
  // Ensure (Remainder * 1,000,000,000) will not overflow 64-bit.
  // Since 2^29 < 1,000,000,000 = 0x3B9ACA00 < 2^30, Remainder should < 2^(64-30) = 2^34,
  // i.e. highest bit set in Remainder should <= 33.
  //
  Shift = MAX (0, HighBitSet64 (Remainder) - 33);
  Remainder = RShiftU64 (Remainder, (UINTN) Shift);
  Frequency = RShiftU64 (Frequency, (UINTN) Shift);
  NanoSeconds += DivU64x64Remainder (MultU64x32 (Remainder, 1000000000u), Frequency, NULL);

  return NanoSeconds;
}

