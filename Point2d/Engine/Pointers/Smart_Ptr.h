#pragma once

namespace Engine
{
	namespace Pointer
	{
		template<class T>
		class WeakPtr;

		template<class T>
		class  SmartPtr
		{
		private:
			

		public:
			T* ptr;
			unsigned long* owner_Count;
			unsigned long* observer_Count;

			//Standard Construtor
			SmartPtr(T* p = nullptr) 
			{
				ptr = p;
				owner_Count = new unsigned long(1);
				observer_Count = new unsigned long(1);
			} 

			//Copy Construtor
			SmartPtr(const SmartPtr& p_other)
			{
				AcquireNewReference(p_other);
			}

			template<class T>
			SmartPtr(WeakPtr<T>& p_other)
			{
				this->ptr = p_other.ptr;
				this->owner_Count = p_other.owner_Count;
				this->observer_Count = p_other.observer_Count;
				(*this->owner_Count)++;
			}

			//Destructor
			~SmartPtr() 
			{
				ReleaseCurrentReference();
			}

			//Overloading Operator*
			T& operator*() 
			{
				return (*ptr); 
			}

			T* operator->() 
			{
				return (ptr); 
			}

			template <class T>
			bool operator==(const SmartPtr<T> p_other) const
			{
				if (ptr == p_other.ptr)
				{
					return true;
				}
				else
					return false;
			}

			unsigned long get_OwnerCount() const
			{
				return (*this->owner_Count);
			}

			unsigned long get_ObserverCount() const
			{
				return (*this->observer_Count);
			}


			//Move Semantics
			SmartPtr& operator=(const SmartPtr& p_other)
			{
				ReleaseCurrentReference();
				AcquireNewReference(p_other);

				return *this;
			}

		private:

			void ReleaseCurrentReference()
			{
				if (--(*owner_Count) == 0)
				{
					delete (ptr);
					delete (owner_Count);
				}
			}

			void AcquireNewReference(const SmartPtr& p_other)
			{
				this->ptr = p_other.ptr;
				this->owner_Count = p_other.owner_Count;
				this->observer_Count = p_other.observer_Count;
				(*this->owner_Count)++;
			}


		};
	}
}


 