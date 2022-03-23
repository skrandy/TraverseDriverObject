#pragma once
#include <ntifs.h>
#define NUMBER_HASH_BUCKETS 0x37

typedef struct _OBJECT_DIRECTORY
{
    struct _OBJECT_DIRECTORY_ENTRY* HashBuckets[37];                        //0x0
    ULONG_PTR  Lock;                                              //0x94
    struct _DEVICE_MAP* DeviceMap;                                          //0x98
    ULONG SessionId;                                                        //0x9c
    VOID* NamespaceEntry;                                                   //0xa0
    ULONG Flags;                                                            //0xa4
}OBJECT_DIRECTORY, * POBJECT_DIRECTORY;


typedef struct _OBJECT_DIRECTORY_ENTRY
{
    struct _OBJECT_DIRECTORY_ENTRY* ChainLink;                              //0x0
    PVOID Object;                                                           //0x4
    ULONG HashValue;                                                        //0x8
}OBJECT_DIRECTORY_ENTRY, * POBJECT_DIRECTORY_ENTRY;