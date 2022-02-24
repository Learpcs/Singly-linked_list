#include <limits>
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
		this->value_ = std::numeric_limits<T>::max();
		this->next_ = nullptr;
	}
	operator T&()
	{
		return this->value_;
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
		this->tail_->next_ = new node<T>(obj);
		this->tail_ = this->tail_->next_;
	}
	void erase_after(iterator it)
	{
		it->next_ = it->next_->next_;
		//(*it).next_ = (*it).next_->next;
	}

	iterator begin()
	{
		return iterator(this->head_);
	}
	iterator end()
	{
		return iterator(nullptr);
	}
};

int main()
{
	list<int> a{ 1, 2, 3 };
	list<int> b;
	b = a;
	a.erase_after(a.begin());
	for (int& x : a)
	{
		std::cout << x << std::endl;
	}
	b.push_back(100);
	b.print();
	std::cin.get();
	std::cout << "lol";
}