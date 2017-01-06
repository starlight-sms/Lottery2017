#pragma once

struct Person
{
	int             WorkId;
	const wchar_t * Name;
	UINT            ResourceId;
	const wchar_t * Notes;

	Person(
		const int id,
		const wchar_t *name,
		const UINT resourceId,
		const wchar_t* notes
	);
};

const std::vector<Person>& GetAllPerson();

std::vector<int> GetUnluckyPersonIds(int itemId);

const void SaveLuckyPersonIds(int itemId, const std::unordered_set<int>& personIds);

std::vector<int> GetLuckyPersonIds(int itemId);

std::vector<Person> PersonIdsToPersons(const std::vector<int>& personIds);

void DeleteLuckyPersons();

CString CreateLuckyStatusFile(int itemId, std::vector<int>& personIds);