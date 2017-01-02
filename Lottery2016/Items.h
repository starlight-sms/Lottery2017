#pragma once

struct Item
{
	const wchar_t * Name;
	const int Count;
	const UINT ResourceId;

	Item(const wchar_t * name, UINT count, int imageId);
};

const std::vector<Item>& GetItems();