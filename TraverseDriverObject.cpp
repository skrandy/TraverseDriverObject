#include "TraverseDriverObject.h"  

void TraverseDriverObject()
{
	/*
	author：SnA1lGo
	time:	2022/3/3
	作用：	遍历\Driver目录对象下的驱动对象

	变量说明：
		Status：				通用返回值
		oa：					打开对象时的状态码
		openedDirectoryName：	被打开的对象名称
		hTempDirectory：		被打开的对象的临时指针
		pDir：					目录对象指针
		pTempDriverObject:		驱动对象指针
		pTempDirEntry:			目录入口对象指针

	函数流程：
		首先打开目录对象得到目录对象句柄，然后通过目录对象句柄得到目录对象指针，然后利用目录对象的结构体特征，
		遍历其HashBuckets数组和数组中的链表来得到对象的内容，然后判断对象是否存在，存在就输出其名称。
	*/
	NTSTATUS Status{ 0 };
	OBJECT_ATTRIBUTES oa{ 0 };
	UNICODE_STRING openedDirectoryName{ 0 };
	HANDLE hDirectory = NULL;
	PVOID  pTempDirectory = NULL;
	POBJECT_DIRECTORY	pDir = NULL;
	PDRIVER_OBJECT pTempDriverObject = NULL;
	POBJECT_DIRECTORY_ENTRY pTempDirEntry = NULL;


	RtlInitUnicodeString(&openedDirectoryName, L"\\Driver");
	InitializeObjectAttributes(&oa, &openedDirectoryName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	Status = ZwOpenDirectoryObject(&hDirectory, 0, &oa);
	if (NT_SUCCESS(Status))
	{
		//得到根目录对象的指针
		Status = ObReferenceObjectByHandle(hDirectory, 0x10000000, 0, 0, &pTempDirectory, 0);
		//如果该API指定了ObjectType，就会比对type是否匹配，如果不指定就不管
		if (!NT_SUCCESS(Status))
		{
			KdPrint(("%Z\n", L"获取\\Driver目录对象失败"));
			return;
		}
		pDir = (POBJECT_DIRECTORY)pTempDirectory;
		//遍历目录对象 \\Driver目录对象
		for (int i = 0; i < NUMBER_HASH_BUCKETS; i++)
		{
			pTempDirEntry = pDir->HashBuckets[i];
			UNICODE_STRING tempName{ 0 };
			while (pTempDirEntry && MmIsAddressValid((PVOID)pTempDirEntry))
			{
				pTempDriverObject = (PDRIVER_OBJECT)pTempDirEntry->Object;
				if (MmIsAddressValid((PVOID)pTempDriverObject))
				{
					if (MmIsAddressValid((PVOID)&pTempDriverObject->DriverName))
					{
						KdPrint(("Driver Name: %wZ\n", &pTempDriverObject->DriverName));
					}

				}
				pTempDirEntry = pTempDirEntry->ChainLink;
			}
		}
		Status = ZwClose(hDirectory);
	}
}

extern "C"
NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObject, _In_ PUNICODE_STRING RegistryPath)
{
	UNREFERENCED_PARAMETER(DriverObject);
	UNREFERENCED_PARAMETER(RegistryPath);
	TraverseDriverObject();
	return STATUS_SUCCESS;
}
