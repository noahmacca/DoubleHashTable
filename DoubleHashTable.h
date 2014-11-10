#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H

		/*****************************************
		* UW User ID:  nwcowanm
		* Submitted for ECE 250
		* Semester of Submission:  Fall 2014
		*
		* By submitting this file, I affirm that
		* I am the author of all modifications to
		* the provided code.
		*****************************************/

 #include "Exception.h"
 #include "ece250.h"
 #include <iostream>

using namespace std;

		enum state { EMPTY, OCCUPIED, DELETED };

		template<typename T>
		class DoubleHashTable {
		private:
			int count;
			int power;
			int array_size;
			T *array;
			state *array_state;

			int h1(T const &) const; // first hash function
			int h2(T const &) const; // second hash function

		public:
			DoubleHashTable(int = 5);
			~DoubleHashTable();
			int size() const;
			int capacity() const;
			bool empty() const;
			bool member(T const &) const;
			T bin(int) const;

			void print() const;

			void insert(T const &);
			bool remove(T const &);
			void clear();
		};

		template<typename T >
		DoubleHashTable<T >::DoubleHashTable(int m) :
			count(0),
			power(m),
			array_size(1 << power),
			array(new T[array_size]),
			array_state(new state[array_size]) {

			for (int i = 0; i < array_size; ++i) {
				array_state[i] = EMPTY;
			}
		}

		template<typename T >
		DoubleHashTable<T >::~DoubleHashTable() {
			// enter your implemetation here 
			// This needs to deallocate the memory allotted for both array and array_state
			delete[] array;
			delete[] array_state;
		}

		template<typename T >
		int DoubleHashTable<T >::size() const {
			// enter your implemetation here 
			return count;
		}

		template<typename T >
		int DoubleHashTable<T >::capacity() const {
			// enter your implemetation here 
			return array_size;
		}



		template<typename T >
		bool DoubleHashTable<T >::empty() const {
			// enter your implemetation here 
			// This will simply tell you if the array is empty or not
			if (count == 0)
			{
				return true;
			}
			else if (count < 0)
			{
				throw out_of_bounds();
			}
			else
			{
				return false;
			}
		}

		template<typename T >
		int DoubleHashTable<T >::h1(T const &obj) const {
			// enter your implemetation here 
			// This is the first hash function, as defined in the course notes
			// As a function, it simply inputs a given value and outputs the hash value for h1
			// We have array_size, use this for m
			int y = (int(obj) % array_size);

			// Add M if the value is negative
			if (y < 0)
			{
				y = (y + array_size);
			}
			return y;
		}

		template<typename T >
		int DoubleHashTable<T >::h2(T const &obj) const {
			// enter your implemetation here 
			// This is the second hash function, as defined in the course notes
			int y = ((int(obj) / array_size) % array_size);

			// Add M if the value is negative
			if (y < 0)
			{
				y = y + array_size;
			}

			// Add 1 if the value is even
			if (y % 2 == 0)
			{
				y = y + 1;
			}

			return y;
		}

		template<typename T >
		bool DoubleHashTable<T >::member(T const &obj) const {
			// enter your implemetation here 
			// Given a value, is it already in the hash table?
			// You can simply apply the first and second hash function
			// If it encounters a match for the value, then true
			// If it encounters an empty slot, then false

			int attemptbin = h1(obj);
			if (array[attemptbin] == obj && array_state[attemptbin] == OCCUPIED)
			{
				return true;
			}
			else
			{
				// Use double hash formula until either we find the desired value or it loops too many times
				int i = 1;
				while (array[attemptbin] != obj | array_state[attemptbin] != OCCUPIED)
				{
					attemptbin = (h1(obj) + h2(obj)*i) % array_size;
					i++;

					if (i > array_size)
					{
						// Your desired value is not in the table
						return false;
					}
				}
				// Now we have found our desired value
				return true;
			}
		}

		template<typename T >
		T DoubleHashTable<T >::bin(int n) const {
			// enter your implementation here 	 
			// This returns the value in a given array location, or "bin" 
			if (0 <= n && n <= array_size)
			{
				T x = array[n];
				return x;
			}
			
			else
			{
				throw out_of_bounds();
			}

		}

		template<typename T >
		void DoubleHashTable<T >::insert(T const &obj) {
			// enter your implemetation here 
			// This adds a new value somewhere into your hash table
			// First it will feed

			// Check if the table is already full
			if (array_size == count)
			{
				// Table is full, we can't add anything here
				throw ArrayBoundsException();
			}
			else
			{
				// There is at least one available slot, so it is possible to insert the value somewhere
				int attemptbin = h1(obj);
				if (array[attemptbin] == obj && array_state[attemptbin] == OCCUPIED)
				{
				//	cout << "This number is already in your hash table" << endl;
					return;
				}

				else if (array_state[attemptbin] != OCCUPIED)
				{
					array_state[attemptbin] = OCCUPIED;
					array[attemptbin] = obj;
					count++;
				}
				else
				{
					// Use double hash formula until either an empty slot is reached or it loops too many times
					int i = 1;
					while (array_state[attemptbin] == OCCUPIED)
					{
						attemptbin = (h1(obj) + h2(obj)*i) % array_size;
						i++;

						if (array[attemptbin] == obj && array_state[attemptbin] == OCCUPIED)
						{
							// cout << "This number is already in your hash table" << endl;
							return;
						}

						if (i > array_size)
						{
							throw overflow();
						}
					}
						// Now we have found a location that is not occupied, so insert	our input object
					array_state[attemptbin] = OCCUPIED;
					array[attemptbin] = obj;
					count++;
					return;
				}
			}
		}

		template<typename T >
		bool DoubleHashTable<T >::remove(T const &obj) {
			// enter your implementation here 	
			// This removes an existing value from your hash table. Be sure to check if the value exists in the hash table to begin with. 
			// Returns true if the value is found in the table and was removed, else false

			// There is at least one element present, so now we need to find the value and remove it

			int attemptbin = h1(obj);
			if (array[attemptbin] == obj && array_state[attemptbin] == OCCUPIED)
			{
				array_state[attemptbin] = DELETED;
				count--;
				return true;
			}
			else
				{
					// Use double hash formula until either we find the desired value or it loops too many times
					int i = 1;
					while (array[attemptbin] != obj | array_state[attemptbin] != OCCUPIED)
					{
						attemptbin = (h1(obj) + h2(obj)*i) % array_size;
						i++;

						if (i > array_size)
						{
							// Your desired value is not in the table, so can't be deleted
							return false;
						}
					}
					// Now we have found our desired value, we just need to update the count, change the array_state value, and return true
					array_state[attemptbin] = DELETED;
					count--;
					return true;
				}

		}



		template<typename T >
		void DoubleHashTable<T >::clear() {
			// enter your implementation here 	
			// set everything in your array state to empty

			for (int j = 0; j < array_size; j++)
			{
				array_state[j] = EMPTY;
				count = 0;
			}
			return;
		}

		template<typename T >
		void DoubleHashTable<T >::print() const {
			// enter your implemetation here 	
			// This is a way of simply looking at your hash table array values. This will allow you to check up on the behavior of the hash table.
			// You can print the values bin by bin, where empty values are zero or "empty" and filled values are printed. 
			// Dave: You can allocate all the values as zero and add any value is not zero

			cout << "value, state:" << endl;
			for (int k = 0; k < array_size; k = k + 1)
			{
				
				if (array_state[k] == OCCUPIED)
				{
					cout << array[k] << "\t";
				}
				else
				{
					cout << "N/A" << "\t";
				}



				if (array_state[k] == OCCUPIED)
				{
					cout << "OCCUPIED" << endl;
				}
				else if (array_state[k] == EMPTY)
				{
					cout << "EMPTY" << endl;
				}
				else
				{
					cout << "DELETED" << endl;
				}

			}
			return;
		}

#endif

