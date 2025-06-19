#ifndef ALLOWTOKEN_H
#define ALLOWTOKEN_H
namespace ac
{
    // Primary template (strictly non-copyable, non-movable, private constructor)
    template<class T>
    class AllowToken
    {
    private:
        AllowToken() = default;
        AllowToken(const AllowToken&) = delete;
        AllowToken(AllowToken&&) = delete;
        AllowToken& operator=(const AllowToken&) = delete;
        AllowToken& operator=(AllowToken&&) = delete;
    };

    // Macro to generate a specialization with friend classes
#define ALLOWTOKEN(T, friendList) \
template<> \
class AllowToken<T> \
{ \
private: \
    AllowToken() = default; \
    AllowToken(const AllowToken&) = delete; \
    AllowToken(AllowToken&&) = delete; \
    AllowToken& operator=(const AllowToken&) = delete; \
    AllowToken& operator=(AllowToken&&) = delete; \
    friendList\
};

#define PUBLIC_ALLOWTOKEN(T) \
template<> \
class AllowToken<T> \
{ \
public: \
    AllowToken() = default; \
    AllowToken(const AllowToken&) = delete; \
    AllowToken(AllowToken&&) = delete; \
    AllowToken& operator=(const AllowToken&) = delete; \
    AllowToken& operator=(AllowToken&&) = delete; \
};
}


#endif // ALLOWTOKEN_H