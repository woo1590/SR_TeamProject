Base::Base() :m_dwRefCnt(0)
{

}

Base::~Base()
{

}

unsigned long Base::AddRef()
{
	return ++m_dwRefCnt;
}

unsigned long Base::Release()
{
	if (m_dwRefCnt == 0)
	{
		Free();
		
		delete this;

		return 0;
	}
	return m_dwRefCnt--;
}