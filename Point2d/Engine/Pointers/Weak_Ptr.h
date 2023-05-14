#pragma once


namespace Engine
{
	namespace Pointer
	{
		template<class T>
		class SmartPtr;

		template<class T>
		class WeakPtr
		{
		private:
			

		public:

			T* ptr;
			unsigned long* owner_Count;
			unsigned long* observer_Count;

			//Standard Constructor
			WeakPtr(T* p = nullptr)
			{
				ptr = p;
				owner_Count = new unsigned long(1);
				observer_Count = new unsigned long(1);
			}

			//Copy Construtor

				//From Smart Pointer
			template<class T>
			WeakPtr(Engine::Pointer::SmartPtr<T>& p_other)
			{
				if (p_other.ptr)
				{
					ptr = p_other.ptr;
					observer_Count = p_other.observer_Count;
					owner_Count = p_other.owner_Count;
					(*observer_Count)++;
				}
				else if (!p_other.ptr)
				{
					this->ptr = nullptr;
					this->owner_Count = nullptr;
					this->observer_Count = nullptr;
				}
			}

				//From Weak Pointer
			template<class T>
			WeakPtr(const WeakPtr& p_other)
			{
				if (p_other.ptr)
				{
					ptr = p_other.ptr;
					observer_Count = p_other.observer_Count;
					owner_Count = p_other.owner_Count;
					(*observer_Count)++;
				}
				else if (!p_other.ptr)
				{
					this->ptr = nullptr;
					this->owner_Count = nullptr;
					this->observer_Count = nullptr;
				}
			}

			//Destructor
			~WeakPtr()
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

			bool IsValid() const
			{
				return (this->ptr != nullptr);
			}

			//Move Semantics
			template<class T>
			WeakPtr& operator=(Engine::Pointer::SmartPtr<T>& p_other)
			{
				ReleaseCurrentReference();
				AcquireNewReference(p_other);

				return *this;
			}

		private:

			void ReleaseCurrentReference()
			{
				(observer_Count)--;
				if (observer_Count == 0)
				{
					if (*owner_Count == 0)
					{
						delete owner_Count;
						delete observer_Count;
					}
				}
			}

			void AcquireNewReference(const WeakPtr& p_other)
			{
				ptr = p_other.ptr;
				observer_Count = p_other.observer_Count;
				owner_Count = p_other.owner_Count;
				(*observer_Count)++;
			}

		};
	}
}