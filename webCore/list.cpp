#include <iostream>
using namespace std;


template <class T> class List{
	public:
		List():m_head(NULL),m_tail(NULL){}
		List(List const& that):m_head(NULL),m_tail(NULL){
		}
		~List(){
		}
		bool empty(){

		}
		void push_front(T const&data){
		}
		void pop_front(){

		}
		T& front(){
		}
		T const& front()const{
		}
		void push_back(T const& data){

		}
		void pop_back(){
		}
		T& back(){
		}
		T const &backe()const{
		}
		void clean(){
		}
		size_t size(){
		}
		friend ostream& operator<<(ostream& os, List const& that){
		}

	private:
	class node{
		public:
			node(node *prev, T const&data, node* next):
			m_prev(prev), m_next(next), m_data(data){}
			node* m_prev;
			node* m_next;
			T m_data;
	};
	node *m_head;
	node *m_tail;
}
