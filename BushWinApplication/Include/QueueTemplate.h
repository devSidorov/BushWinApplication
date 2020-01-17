#pragma once
#include <memory.h>

template <class T>
struct NodeTemplate
{
	T m_data;
	NodeTemplate* m_pNextLink;

	NodeTemplate();
};

template <class T>
class QueueLinkedList
{
private:
	NodeTemplate* m_pLastNode;
	NodeTemplate* m_pFirstNode;

private:
	void fnAddEmptyNode();

public:
	QueueLinkedList();
	QueueLinkedList( T& data );
	~QueueLinkedList();

	virtual bool fnIsEmpty();
	virtual void fnPush( T& data );
	virtual void fnPop( T& data );
};

template <class T>
class LimitedQueue : private QueueLinkedList< T >
{
private:
	unsigned int m_limitPos;
	unsigned int m_queueSize;

public:
	LimitedQueue( unsigned int limit = NULL ) : m_limitSize( limit ), m_queueSize( NULL ) {};
	LimitedQueue( T& data, unsigned int limit = NULL ) : QueueLinkedList( data ), 
		m_limitSize( limit ), m_queueSize( 1 ) {};

	virtual bool fnIsEmpty();
	virtual void fnPush( T& data );
	virtual void fnPop( T& data );

};


