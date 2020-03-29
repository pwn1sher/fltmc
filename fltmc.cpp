// Pwnisher

#include <Windows.h>
#include <iostream>
#include <fltUser.h>
#include <fltUserStructures.h>

#pragma comment(lib, "fltLib.lib")

void printFilter(_In_ PVOID Buffer) {
  CHAR FilterName[128] = {0};
  CHAR Altitude[64] = {0};
  PFILTER_AGGREGATE_STANDARD_INFORMATION FilterAggInfo;
  FilterAggInfo = (PFILTER_AGGREGATE_STANDARD_INFORMATION)Buffer;

  if (FilterAggInfo->Type.MiniFilter.FilterNameLength < 128) {
    CopyMemory(FilterName,
               (PCHAR)FilterAggInfo +
                   FilterAggInfo->Type.MiniFilter.FilterNameBufferOffset,
               FilterAggInfo->Type.MiniFilter.FilterNameLength);
    FilterName[FilterAggInfo->Type.MiniFilter.FilterNameLength] = UNICODE_NULL;
  }

  if (FilterAggInfo->Type.MiniFilter.FilterNameLength < 64) {
    CopyMemory(Altitude,
               (PCHAR)FilterAggInfo +
                   FilterAggInfo->Type.MiniFilter.FilterAltitudeBufferOffset,
               FilterAggInfo->Type.MiniFilter.FilterAltitudeLength);
    FilterName[FilterAggInfo->Type.MiniFilter.FilterAltitudeLength] =
        UNICODE_NULL;
  }

  wprintf(L"%-38s %-10lu %-10s %-10lu\n", FilterName,
          FilterAggInfo->Type.MiniFilter.NumberOfInstances, Altitude,
          FilterAggInfo->Type.MiniFilter.FrameID);
}

int main() {
  HANDLE FindHandle;
  BYTE Buffer[1024];
  ULONG BytesReturned;
  BOOL IsNewStyle = TRUE;
  HRESULT hr;

  hr = FilterFindFirst(FilterAggregateStandardInformation, Buffer, 1024,
                       &BytesReturned, &FindHandle);

  printFilter(Buffer);

  for (int i = 0; i < 8; i++) {
    hr = FilterFindNext(FindHandle, FilterAggregateStandardInformation, Buffer,
                        1024, &BytesReturned);

    printFilter(Buffer);
  }

  return 0;
}
