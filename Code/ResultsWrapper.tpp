template<typename T>
void ResultsWrapper<T>::emit(const T& result)
{
	v.push_back(std::move(result));
}

template<typename T>
T ResultsWrapper<T>::at(const unsigned int index)
{
	return v[index];
}

template<typename T>
T ResultsWrapper<T>::first()
{
	return v[0];
}

template<typename T>
T ResultsWrapper<T>::last()
{
	return v.back();
}

template<typename T>
void ResultsWrapper<T>::delete_last()
{
	v.pop_back();
}

template<typename T>
T* ResultsWrapper<T>::data()
{
	return v.data();
}

template<typename T>
unsigned int ResultsWrapper<T>::size()
{
	return v.size();
}

template<typename T>
typename ResultsWrapper<T>::iterator ResultsWrapper<T>::begin() 
{
  return v.begin();
}

template<typename T>
typename ResultsWrapper<T>::iterator ResultsWrapper<T>::end() 
{
  return v.end();
}

template<typename T>
typename ResultsWrapper<T>::reverse_iterator ResultsWrapper<T>::rbegin() 
{
  return v.rbegin();
}

template<typename T>
typename ResultsWrapper<T>::reverse_iterator ResultsWrapper<T>::rend() 
{
  return v.rend();
}
