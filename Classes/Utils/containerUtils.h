#pragma once

template<typename T>
void pushSorted(std::vector<T*>* res, T* elem, bool(*cmp)(T*,T*))
{
	auto it = res->begin();
	bool ins = false;
	for (auto &ent : *res)
	{
		if(cmp(ent, elem))
		{
			res->insert(it, elem);
			ins = true;
			break;
		}
		it++;
	}
	if (!ins)
	{
		res->push_back(elem);
	}
}