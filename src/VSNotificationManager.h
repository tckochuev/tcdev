#pragma once

#include <utility>

template<typename TDescriptor, typename TContainer>
class VSINotificationManager
{
public:
	using Descriptor = TDescriptor;
	using Container = TContainer;

	template<typename Manager>
	class DescriptorsReference
	{
	public:
		decltype((&std::declval<Manager&>().m_descriptors)) operator->() {
			return &(mgr.m_descriptors);
		}

	private:
		DescriptorsReference(Manager& mgr) : mgr(mgr) {}

		Manager& mgr;

		friend class VSINotificationManager;
	};

	using DescRef = DescriptorsReference<VSINotificationManager>;
	using DescConstRef = DescriptorsReference<const VSINotificationManager>;

	virtual DescRef descriptors() {
		return {*this};
	}
	virtual DescConstRef descriptors() const {
		return {*this};
	}

protected:
	Container m_descriptors;
};