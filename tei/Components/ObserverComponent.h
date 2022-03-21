#pragma once

#include "Subject.h"

namespace tei::internal::components
{

	class ObserverComponent
	{
	public:
		
		template <detail::Observer Observer>
		auto& AddObserver(Subject& subject, Observer observer = {})
		{
			auto& data = subject.AddObserver(std::move(observer));
			m_Observers[&subject].push_back(data);
			return data;
		}

		void Clear();

	private:

		std::unordered_map<Subject*, std::vector<utility::AnyRef>> m_Observers{};
	
	};

}

namespace tei::external::components
{
	using tei::internal::components::ObserverComponent;
}

void OnDisable(tei::internal::Internal, tei::internal::components::ObserverComponent&);