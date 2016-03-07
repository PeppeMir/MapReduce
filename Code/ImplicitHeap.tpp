template<typename T>
void ImplicitHeap<T>::insert(T* item)
{
	heap.push_back(item);
	
	if (!created)
		return;
	
	push_heap(heap.begin(), heap.end(), compare);
}

template<typename T>
T* ImplicitHeap<T>::extract()
{
	if (!created)
	{
		make_heap(heap.begin(), heap.end(), compare);
		created = true;
	}
	
	T* item = heap.front();
	pop_heap(heap.begin(), heap.end(), compare); 
	heap.pop_back();
	
	return item;
}

template<typename T>
T* ImplicitHeap<T>::peek()
{
	return heap.front();
}

template<typename T>
unsigned int ImplicitHeap<T>::size()
{
	return heap.size();
}

template<typename T>
typename ImplicitHeap<T>::iterator ImplicitHeap<T>::begin() 
{
  return heap.begin();
}

template<typename T>
typename ImplicitHeap<T>::iterator ImplicitHeap<T>::end() 
{
  return heap.end();
}

template<typename T>
typename ImplicitHeap<T>::const_iterator ImplicitHeap<T>::cbegin() 
{
  return heap.cbegin();
}

template<typename T>
typename ImplicitHeap<T>::const_iterator ImplicitHeap<T>::cend() 
{
  return heap.cend();
}
