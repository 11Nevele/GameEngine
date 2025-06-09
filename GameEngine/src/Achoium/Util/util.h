#pragma once
namespace ac
{
	namespace util
	{
		std::string ReadFile(const std::string& path);
		template<class T>
		std::shared_ptr<T> Ref(T& t)
		{
			return make_shared<T>(t);
		}
	}
}
