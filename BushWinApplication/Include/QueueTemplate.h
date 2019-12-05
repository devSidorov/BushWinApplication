#pragma once

template <class T>
struct NodeTemplate
{
	T m_data;
	NodeTemplate* m_pNextLink;
};

template <class T>
class QueueLinkedList
{
private:
	NodeTemplate* m_pCurrentNode;
	NodeTemplate* m_pFirstNode;
	

public:
	QueueLinkedList();
	QueueLinkedList( T data );
	~QueueLinkedList();

	void fnAddEmptyNode();
	void fnAddNode( T data );

	void fnGetDataInQueue( T& data );

};


