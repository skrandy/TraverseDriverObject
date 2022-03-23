#include "TraverseDriverObject.h"  

void TraverseDriverObject()
{
	/*
	author��SnA1lGo
	time:	2022/3/3
	���ã�	����\DriverĿ¼�����µ���������

	����˵����
		Status��				ͨ�÷���ֵ
		oa��					�򿪶���ʱ��״̬��
		openedDirectoryName��	���򿪵Ķ�������
		hTempDirectory��		���򿪵Ķ������ʱָ��
		pDir��					Ŀ¼����ָ��
		pTempDriverObject:		��������ָ��
		pTempDirEntry:			Ŀ¼��ڶ���ָ��

	�������̣�
		���ȴ�Ŀ¼����õ�Ŀ¼��������Ȼ��ͨ��Ŀ¼�������õ�Ŀ¼����ָ�룬Ȼ������Ŀ¼����Ľṹ��������
		������HashBuckets����������е��������õ���������ݣ�Ȼ���ж϶����Ƿ���ڣ����ھ���������ơ�
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
		//�õ���Ŀ¼�����ָ��
		Status = ObReferenceObjectByHandle(hDirectory, 0x10000000, 0, 0, &pTempDirectory, 0);
		//�����APIָ����ObjectType���ͻ�ȶ�type�Ƿ�ƥ�䣬�����ָ���Ͳ���
		if (!NT_SUCCESS(Status))
		{
			KdPrint(("%Z\n", L"��ȡ\\DriverĿ¼����ʧ��"));
			return;
		}
		pDir = (POBJECT_DIRECTORY)pTempDirectory;
		//����Ŀ¼���� \\DriverĿ¼����
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
