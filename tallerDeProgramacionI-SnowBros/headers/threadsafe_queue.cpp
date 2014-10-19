#include <queue>
#include <memory>
#include <mutex>
#include <condition_variable>

template<typename T>
class Threadsafe_queue{
private:
        mutable std::mutex mut;         // Since locking a mutex is a mutating operation, the mutex object must be marked mutable so it can be locked in empty() and in the copy constructor
        std::queue<T> data_queue;
        std::condition_variable data_cond;

public:
        Threadsafe_queue(){}

        Threadsafe_queue(Threadsafe_queue const& other){
                std::lock_guard<std::mutex> lk(other.mut);
                data_queue = other.data_queue;
        }

        void push(T new_value){
			std::lock_guard<std::mutex> lk(mut);
			data_queue.push(new_value);
			data_cond.notify_one();
        }

        /*
         * When using a queue to pass data between threads, the receiving thread often needs to wait for the data.
         * So this method will wait until there's a value to retrieve.
         * Post: Stores the retrieved value in the referenced variable
         *
         */
        void wait_and_pop(T& value){
			std::unique_lock<std::mutex> lk(mut);
			data_cond.wait(lk,[this]{return !data_queue.empty();});
			value=data_queue.front();
			data_queue.pop();
        }

        T wait_and_pop(){
			std::unique_lock<std::mutex> lk(mut);
			data_cond.wait(lk,[this]{return !data_queue.empty();});
			T res(data_queue.front());
			data_queue.pop();
			return res;
        }

        /*
         * The only difference between this and wait_and_pop is that this method tries to pop the value from the queue
         * but always returns immediately even if there wasn't a value to retrieve.
         *
         * So this method Stores the retrieved value in the referenced variable, so it can return value for status.
         * It returns true if it retrieved a value or false otherwise
         */
        bool try_pop(T& value){
                std::lock_guard<std::mutex> lk(mut);
                if(data_queue.empty())
                        return false;
                value=data_queue.front();
                data_queue.pop();
                return true;
        }

        /*
         * It returns the retrieved value directly, but the returned value can be set to NULL if there's no value to retrieve
         */
        std::shared_ptr<T> try_pop(){
                std::lock_guard<std::mutex> lk(mut);
                if(data_queue.empty())
                        return std::shared_ptr<T>();
                std::shared_ptr<T> res(std::make_shared<T>(data_queue.front()));
                data_queue.pop();
                return res;
        }

        bool empty() const{
                std::lock_guard<std::mutex> lk(mut);
                return data_queue.empty();
        }
};
