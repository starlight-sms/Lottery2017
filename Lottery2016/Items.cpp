#include "pch.h"
#include "Items.h"
#include "Resources\resource.h"

using namespace std;

Item::Item(const wchar_t * name, UINT count, int imageId) :
	Name(name),
	Count(count),
	ResourceId(imageId)
{
}

const std::vector<Item>& GetItems()
{
	static vector<Item> result =
	{
		Item(L"一等奖",      1,  IDR_ITEM1 + 0),
		Item(L"二等奖",      5,  IDR_ITEM1 + 1),
		Item(L"三等奖",      10, IDR_ITEM1 + 2),
		Item(L"开心奖",      30, IDR_ITEM1 + 3),
		Item(L"特别奖-AAA",  1,  IDR_ITEM1 + 4),
		Item(L"特别奖-BBB",  1,  IDR_ITEM1 + 5),
		Item(L"特别奖-CCC",  1,  IDR_ITEM1 + 6),
		Item(L"特别奖-DDD",  1,  IDR_ITEM1 + 7),
	};
	return result;
}
