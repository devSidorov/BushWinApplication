#include "QueueTemplate.h"

template<class T>
QueueLinkedList<T>::QueueLinkedList()
{
	m_pFirstNode = m_pCurrentNode = nullptr;
}

template<class T>
QueueLinkedList<T>::QueueLinkedList( T data )
{
	m_pFirstNode = m_pCurrentNode = new NodeTemplate;
	m_pCurrentNode->m_data = data;
	m_pCurrentNode->m_pNextLink = nullptr;
}

template<class T>
QueueLinkedList<T>::~QueueLinkedList()
{
	NodeTemplate* pTempNode = nullptr;
	
	while ( m_pCurrentNode ) //when it comes to nullptr its last link, memory clean
	{
		pTempNode = m_pCurrentNode;
		m_pCurrentNode = m_pCurrentNode->m_pNextLink;
		delete pTempNode;
	}
}

template<class T>
void QueueLinkedList<T>::fnAddEmptyNode()
{
	m_pCurrentNode->m_pNextLink = new NodeTemplate;
	m_pCurrentNode = m_pCurrentNode->m_pNextLink;
}

template<class T>
void QueueLinkedList<T>::fnAddNode( T data )
{
	fnAddEmptyNode();
	m_pCurrentNode->m_data = data;
}

template<class T>
void QueueLinkedList<T>::fnGetDataInQueue( T& data )
{
	NodeTemplate* pProcessedNode = m_pFirstNode;
	
	data = m_pFirstNode->m_data;

	m_pFirstNode = m_pFirstNode->m_pNextLink;
	delete pProcessedNode;
}

