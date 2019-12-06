#include "QueueTemplate.h"

template<class T>
NodeTemplate<T>::NodeTemplate()
{
	memset( *m_data, NULL, sizeof( T ) );
	m_pNextLink = nullptr;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
QueueLinkedList<T>::QueueLinkedList()
{
	m_pFirstNode = m_pLastNode = nullptr;
}

template<class T>
QueueLinkedList<T>::QueueLinkedList( T& data )
{
	m_pFirstNode = m_pLastNode = new NodeTemplate;
	m_pLastNode->m_data = data;
	m_pLastNode->m_pNextLink = nullptr;
}

template<class T>
QueueLinkedList<T>::~QueueLinkedList()
{
	NodeTemplate* pTempNode = nullptr;
	
	while ( m_pLastNode ) //when it comes to nullptr its last link, memory clean
	{
		pTempNode = m_pLastNode;
		m_pLastNode = m_pLastNode->m_pNextLink;
		delete pTempNode;
	}
}

template<class T>
void QueueLinkedList<T>::fnAddEmptyNode()
{
	if ( fnIsEmpty )
	{
		m_pLastNode = m_pFirstNode = new NodeTemplate;
	}
	else
	{
		m_pLastNode->m_pNextLink = new NodeTemplate;
		m_pLastNode = m_pLastNode->m_pNextLink;
	}
}

template<class T>
bool QueueLinkedList<T>::fnIsEmpty()
{
	return ( m_pFirstNode == nullptr );
}

template<class T>
void QueueLinkedList<T>::fnPush( T& data )
{
	fnAddEmptyNode();
	m_pLastNode->m_data = data;
}

template<class T>
void QueueLinkedList<T>::fnPop( T& data )
{
	NodeTemplate* pProcessedNode = m_pFirstNode;
	
	data = m_pFirstNode->m_data;

	m_pFirstNode = m_pFirstNode->m_pNextLink;
	if ( fnIsEmpty )
		m_pLastNode = nullptr;
	delete pProcessedNode;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////

template<class T>
bool LimitedQueue<T>::fnIsEmpty()
{
	return QueueLinkedList< T >::fnIsEmpty();
}

template<class T>
void LimitedQueue<T>::fnPush( T & data )
{
	if ( m_limitPos )
		while ( m_queueSize >= m_limilPos )
		{
			T tempData;
			QuueLinkedList< T >::fnPop( tempData );
			--m_queueSize;
		}

	QuueLinkedList< T >::fnPush( data );
	++m_queueSize;
}

template<class T>
void LimitedQueue<T>::fnPop( T & data )
{
}


