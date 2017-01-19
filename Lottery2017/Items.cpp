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
		Item(L"一等奖-iPhone7",						1,  IDR_ITEM1),
		Item(L"二等奖-机械键盘x2/Kindlex3",			5,  IDR_ITEM2),
		Item(L"三等奖-小米净化器x5/呼吸套装x5",		10, IDR_ITEM4),
		Item(L"阳光普照奖x30-小熊酸奶机",				30, IDR_ITEM6),
		Item(L"阳光普照奖x30-弓箭全钢化保鲜盒套装",	30, IDR_ITEM7),
		Item(L"阳光普照奖x30-立白洗衣液",				30, IDR_ITEM8),
		Item(L"特别奖-丁总-戴尔27寸曲面屏显示器",		1,  IDR_ITEM9),
		Item(L"特别奖-李茂-京东e卡",					1,  IDR_ITEM10),
		Item(L"特别奖-杨秀冬-iPad mini2",			1,  IDR_ITEM11),
		Item(L"特别奖-吴晓妍-SKⅡ神仙水 330ml",		1,  IDR_ITEM12),
	};
	return result;
}
