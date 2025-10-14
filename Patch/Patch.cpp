/**
*  yrpp-spawner
*
*  Copyright(C) 2022-present CnCNet
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "Patch.h"
#include "Macro.h"

void Patch::Apply()
{
	void* pAddress = (void*)this->offset;

	DWORD protect_flag;
	VirtualProtect(pAddress, this->size, PAGE_EXECUTE_READWRITE, &protect_flag);
	memcpy(pAddress, this->pData, this->size);
	VirtualProtect(pAddress, this->size, protect_flag, NULL);
}

void Patch::Apply_LJMP(DWORD offset, DWORD pointer)
{
	const _LJMP data(offset, pointer);
	Patch patch = { offset, sizeof(data), (byte*)&data };
	patch.Apply();
}

void Patch::Apply_CALL(DWORD offset, DWORD pointer)
{
	const _CALL data(offset, pointer);
	Patch patch = { offset, sizeof(data), (byte*)&data };
	patch.Apply();
}

void Patch::Apply_CALL6(DWORD offset, DWORD pointer)
{
	const _CALL6 data(offset, pointer);
	Patch patch = { offset, sizeof(data), (byte*)&data };
	patch.Apply();
}
