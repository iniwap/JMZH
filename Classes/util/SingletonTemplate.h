/**
*
*����ģʽ��ģ��������ʵ�ֵ���
*���У���̬�������ڱ��浥�������÷������ڻ�ȡ�������ͷ�
*
*/
#ifndef __SINGLETON_TEMPLATE_H__
#define __SINGLETON_TEMPLATE_H__

template<class Type>
class SingletonTemplate
{
public:
	//��ȡ����
	static inline Type* instance();
	//�ͷŵ���
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

//ʹ�õ�cpp�ļ���Ҫ������̬����
#define DECLARE_SINGLETONTEMPLATE_MEMBER(_Type)\
	template <> _Type* SingletonTemplate<_Type>::_instance = 0;

#endif  // __SINGLETON_TEMPLATE_H__