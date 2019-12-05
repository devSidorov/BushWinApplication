#pragma once
template <class T>
struct LinkTemplate
{
	T m_data;
	LinkTemplate* m_pNextLink;
};

template <class T>
class QueryTemplate
{
private:
	LinkTemplate* m_pFirstLink;
	LinkTemplate* m_pLastLink;

public:
	QueryTemplate();
	~QueryTemplate();

};


