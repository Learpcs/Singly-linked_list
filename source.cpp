#include <climits>
#include <algorithm>
#include <iostream>
#include <initializer_list>

template <typename T>
struct node
{
	T value_;
	node* next_;

	node(const T& n)
	{
		this->value_ = n;
		this->next_ = nullptr;
	}
	node()
	{
		this->invalidate();
	}
	~node()
	{
		this->invalidate();
	}
	operator T&()
	{
		return this->value_;
	}

private:
	void invalidate()
	{
		this->value_ = std::numeric_limits<T>::max();
		this->next_ = nullptr;
	}
};

template <typename T>
class list
{
public:

	class MyIterator
	{
	public:

		MyIterator(node<T>* ptr)
			: m_Ptr(ptr) {}

		MyIterator& operator++()
		{
			this->m_Ptr = this->m_Ptr->next_;
			return *this;
		}
		MyIterator operator++(int)
		{
			MyIterator copy(*this);
			this->operator++();
			return copy;
		}

		bool operator==(const MyIterator& rhs)
		{
			return this->m_Ptr == rhs.m_Ptr;
		}
		bool operator!=(const MyIterator& rhs)
		{
			return this->m_Ptr != rhs.m_Ptr;
		}

		node<T>& operator*()
		{
			return *this->m_Ptr;
		}

		node<T>* operator->()
		{
			return m_Ptr;
		}
		  
	private:
		node<T>* m_Ptr;
	};

	using iterator = MyIterator;
private:
	node<T>* head_;
	node<T>* tail_;

	size_t size_;

	void swap(list& l1)
	{
		std::swap(l1.size_, this->size_);
		std::swap(l1.head_, this->head_);
		std::swap(l1.tail_, this->tail_);
	}
	void invalidate()
	{
		this->head_ = nullptr;
		this->tail_ = nullptr;
		this->size_ = 0;
		this->~list();
	}


public:
	list()
	{
		this->invalidate();
	}
	list(std::initializer_list<T> il) : list()
	{
		this->size_ = il.size();
		if (il.size() > 0)
		{
			auto it = il.begin();
			this->head_ = new node<T>(*it);
			++it;
			node<T>* current = this->head_;
			for (; it != il.end(); ++it)
			{
				current->next_ = new node<T>(*it);
				current = current->next_;
			}
		}
	}
	list(const list& arr) : list()
	{
		this->size_ = arr.size_;
		if (arr.head_ == nullptr)
			this->head_ = nullptr;
		else
			this->head_ = new node<T>(arr.head_->value_);
		node<T>* current_this = this->head_;
		node<T>* current_arr = arr.head_->next_;
		while (current_arr != nullptr)
		{
			current_this->next_ = new node<T>(current_arr->value_);
			current_this = current_this->next_;
			current_arr = current_arr->next_;
		}
		this->tail_ = current_this;
	}
	list(list&& arr) : list()
	{
		this->swap(arr);

#ifdef _DEBUG
		arr.invalidate();
#endif //_DEBUG
	}
	~list()
	{
		node<T>* current = this->head_;
		while (current != nullptr)
		{
			node<T>* next_ = current->next_;
			delete current;
			current = next_;
		}
	}
	list& operator=(const list& arr)
	{
		this->~list();
		this->size_ = arr.size_;
		if (arr.head_ == nullptr)
			this->head_ = nullptr;
		else
			this->head_ = new node<T>(arr.head_->value_);
		node<T>* current_this = this->head_;
		node<T>* current_arr = arr.head_->next_;
		while (current_arr != nullptr)
		{
			current_this->next_ = new node<T>(current_arr->value_);
			current_this = current_this->next_;
			current_arr = current_arr->next_;
		}
		this->tail_ = current_this;
		return *this;
	}
	list& operator=(list&& arr) noexcept
	{
		this->swap(arr);

#ifdef _DEBUG
		arr.invalidate();
#endif //_DEBUG

		return *this;
	}

	void print() const
	{
		node<T>* current = this->head_;
		while (current != nullptr)
		{
			std::cout << current->value_ << ' ';
#ifdef _DEBUG
			std::cout << "_ ";
#endif // _DEBUG
			current = current->next_;
		}
	}
	void push_back(const T& obj)
	{
		if (this->size_ == 0)
		{
			this->head_ = new node<T>(obj);
			this->tail_ = this->head_;
		}
		else
		{
			this->tail_->next_ = new node<T>(obj);
			this->tail_ = this->tail_->next_;
		}
		++this->size_;
	}
	void erase_after(iterator& it)
	{
		it->next_ = it->next_->next_;
		--this->size_;
	}
	void erase(const T& value)
	{
		node<T>* current = this->head_;
		while (current == this->head_ && current != nullptr)
		{
			if (value == current->value_)
			{
				this->pop_front();
				current = this->head_;
				--this->size_;
			}
			else 
			{
				current = current->next_;
			}
		}
		node<T>* prev = this->head_;
		while (current != nullptr)
		{
			if (current->value_ == value)
			{
				prev->next_ = current->next_;
				current = current->next_;
				--this->size_;
			}
			else
			{
				prev = current;
				current = current->next_;
			}
		}
	}
	void pop_front()
	{
		node<T>* current = this->head_;
		this->head_ = this->head_->next_;
		delete current;
		--this->size_;
	}
	void insert_after(iterator& it, const T& value)
	{
		node<T>* tmp = new node<T>(value);
		tmp->next_ =  it->next_;
		it->next_ = tmp;
	}
	size_t size() const
	{
		return this->size_;
	}


	iterator begin() const
	{
		return iterator(this->head_);
	}
	iterator end() const
	{
		return iterator(nullptr);
	}
};

int main()
{
	list<int> a;
	int x;
	while (std::cin >> x)
		a.push_back(x);

	for (auto it = a.begin(); it != a.end(); ++it)
	{
		if (*it % 2 == 0)
		{
			a.insert_after(it, *it);
			++it;
		}
	}
	for (int& x : a)
		std::cout << x << ' ';
}