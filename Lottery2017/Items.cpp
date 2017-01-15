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
		Item(L"二等奖x2-机械键盘",					2,  IDR_ITEM2),
		Item(L"二等奖x3-Kindle",						3,  IDR_ITEM3),
		Item(L"三等奖x5-小米净化器",					5,  IDR_ITEM4),
		Item(L"三等奖x5-呼吸套装",					5,  IDR_ITEM5),
		Item(L"阳光普照奖x30-小熊酸奶机",				30, IDR_ITEM6),
		Item(L"阳光普照奖x30-弓箭全钢化保鲜盒套装",	30, IDR_ITEM7),
		Item(L"阳光普照奖x30-立白洗衣液",				30, IDR_ITEM8),
		Item(L"特别奖-丁总-戴尔27寸曲面屏显示器",		1,  IDR_ITEM9),
		Item(L"特别奖-李茂-京东e卡",					1,  IDR_ITEM10),
		Item(L"特别奖-杨秀东-未定",					1,  IDR_ITEM11),
		Item(L"特别奖-吴晓研-SKⅡ神仙水 330ml",		1,  IDR_ITEM12),
	};
	return result;
}
