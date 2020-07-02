//Code by Rogier Walraven. Do not distribute.

template<class T>
struct ARRAY
{
	ARRAY(int cap);
	~ARRAY();

	int getcapacity() const;

	T& operator[](int i);

  private:

	int capacity;

	T *elements;

	ARRAY(const ARRAY<T> &other) {}
	ARRAY<T>& operator=(const ARRAY<T> &other) {}
};

template<class T>
inline ARRAY<T>::ARRAY(int cap) :
	capacity(cap),
	elements(new T[cap]) {}

template<class T>
inline ARRAY<T>::~ARRAY()
{
	delete[] elements;
}

template<class T>
inline int ARRAY<T>::getcapacity() const
{
	return(capacity);
}

template<class T>
inline T& ARRAY<T>::operator[](int i)
{
	if (i>=capacity)
	{
		int newcapacity=capacity;

		while (newcapacity<=i)
		{
			newcapacity*=2;
		}

		T *newelements=new T[newcapacity];

		for (int i=0;i<capacity;i++)
		{
			newelements[i]=elements[i];
		}

		delete[] elements;

		capacity=newcapacity;

		elements=newelements;
	}

	return(elements[i]);
}
