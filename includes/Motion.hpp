#ifndef SCOP_MOTION_HPP
#define SCOP_MOTION_HPP
#include <queue>
#include <chrono>
#include <list>
#include <functional>
#include <iostream>
#include <set>

template <class T>
class Motion {
public:
	using time_point = std::chrono::time_point<std::chrono::system_clock>;
	struct transition {
		time_point		begin;
		time_point		end;
		T				*ref;
//		int 			type;
		T				from;
		T				to;
	};
private:
	class unique_transition_compare{
	public:
		bool operator()(transition const &l, transition const &r) const {
			return l.ref < r.ref;
		}
	};
	std::set<transition, unique_transition_compare>	ongoing;
	std::priority_queue<transition>	future;
public:
	void add_transition(T *ref, T from, T to, time_point begin = time_point::clock::now(), time_point end = time_point::clock::now())
	{
		transition t =
				{
						begin,
						end,
						ref,
						from,
						to
				};
//		std::cout << "from: " << from << " to: " << to << std::endl;
		this->future.push(t);
		this->update();
	}

	// speed is in unit per duration
	void	add_transition(T *ref, T to, time_point::duration speed)
	{
		T from = *ref;
		if (to == from)
			return;
		this->add_transition(ref, from, to, time_point::clock::now(), time_point::clock::now() + time_point::duration((long int)(std::abs(to - from) * speed.count())));
	}

	void	update()
	{
		time_point now = std::chrono::system_clock::now();
		while (!this->future.empty() && this->future.top().begin < now) {
			auto ret = this->ongoing.insert(this->future.top());
			if (!ret.second)
			{
				this->ongoing.erase(ret.first);
				this->ongoing.insert(this->future.top());
			}
			this->future.pop();
		}
//				std::cout << this->future.size() << " " << this->ongoing.size() << std::endl;

		for (auto it = this->ongoing.begin(); it != this->ongoing.end();)
		{
			auto &t = *it;
			auto next = std::next(it);
			if (t.end < now)
			{
				*t.ref = t.to;
				this->ongoing.erase(it);
//				std::cout << "erase" << std::endl;
			}
			else
			{
					*t.ref = t.from + (((t.to - t.from) * (float)(now - t.begin).count()) / (float)(t.end - t.begin).count());
//				std::cout << *t.ref << std::endl;
			}
			it = next;
		}
	}

	friend bool operator<(transition const &l, transition const &r) {
		return l.begin > r.begin;
	}

};


#endif //SCOP_MOTION_HPP
