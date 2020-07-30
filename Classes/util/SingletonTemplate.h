/**
*
*单例模式类模板容器，实现单例
*其中：静态变量用于保存单例；公用方法用于获取单例和释放
*
*/
#ifndef __SINGLETON_TEMPLATE_H__
#define __SINGLETON_TEMPLATE_H__

template<class Type>
class SingletonTemplate
{
public:
	//获取单例
	static inline Type* instance();
	//释放单例
	void release();
protected:
	SingletonTemplate(void){}
	~SingletonTemplate(void){}
	static Type* _instance;
};

template<class Type>
inline Type* SingletonTemplate<Type>::instance()
{
	if(!_instance)
	{
		_instance = new Type;
	}
	return _instance;
}

template <class Type>
void SingletonTemplate<Type>::release()
{
	if(!_instance)
	{
		return;
	}
	delete _instance;
	_instance = 0;
}

//使用到cpp文件需要声明静态变量
#define DECLARE_SINGLETONTEMPLATE_MEMBER(_Type)\
	template <> _Type* SingletonTemplate<_Type>::_instance = 0;

#endif  // __SINGLETON_TEMPLATE_H__