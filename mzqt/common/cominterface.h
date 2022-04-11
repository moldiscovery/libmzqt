/*
 cominterface.hpp
 Created on: 24/11/2010
 Author: Fabien Fontaine
 */

#ifndef MZQT_COMINTERFACE_HPP_
#define MZQT_COMINTERFACE_HPP_

#include <QHash>
#include <QByteArray>
#include <QList>

#include <OCIdl.h>
//#include <windef.h>

#include <mzqt/common/Exception.h>

#define THROW_MZQT_IDISPATCHEXCEPT_HRES(hres) (throw mzqt::DispatchException(hres, "", __FILE__, __LINE__))
#define THROW_MZQT_IDISPATCHEXCEPT_MSG(msg) (throw mzqt::DispatchException(msg, __FILE__, __LINE__))


namespace mzqt {

  class DispatchException: public Exception {

  public:

    explicit DispatchException(HRESULT hres, const std::string &msg = "",
        const char *file = 0, int line = 0);
    explicit DispatchException(const std::string &msg = "", const char *file =
        0, int line = 0);
    virtual ~DispatchException() throw ();
  };

  class COMInterface {

  protected:
    DISPID dispIDofName(const QByteArray &name, IDispatch *disp);
    void getNum(const char *dispName, int &e);

    IDispatch *disp_;
    QHash<QByteArray, DISPID> dispIDs_;
  };

#if 0
  inline BSTR QStringToBSTR(const QString &str)
  {
    return SysAllocStringLen((OLECHAR*) str.unicode(), str.length());
  }
#endif

  inline QString BSTRToQString(const BSTR &bstr)
  {
    return QString::fromUtf16((const ushort *) bstr);
  }


  template<typename T>
  HRESULT SAFEARRAYToQList(SAFEARRAY *pArray, QList<T> &list)
  {
    list.clear();

    if(!pArray)
      return S_OK; //nothing to do

    HRESULT hr;
    long upperBound = 0;
    hr = SafeArrayGetUBound(pArray, 1, &upperBound);
    if (FAILED(hr)) {
      return hr;
    }
    long lowerBound = 0;
    hr = SafeArrayGetLBound(pArray, 1, &lowerBound);
    if (FAILED(hr)) {
      return hr;
    }

    T* values;
    SafeArrayAccessData(pArray, reinterpret_cast<void**> (&values));
    for (; lowerBound <= upperBound; lowerBound++) {
      T v = values[lowerBound];
      list.push_back(v);
    }
    SafeArrayUnaccessData(pArray);

    return S_OK;
  }

  template<typename T>
  HRESULT SAFEARRAYToQVector(SAFEARRAY *pArray, QVector<T> &vector)
  {
    vector.clear();

    if(!pArray)
      return S_OK; //nothing to do

    HRESULT hr;
    long upperBound = 0;
    hr = SafeArrayGetUBound(pArray, 1, &upperBound);
    if (FAILED(hr)) {
      return hr;
    }
    long lowerBound = 0;
    hr = SafeArrayGetLBound(pArray, 1, &lowerBound);
    if (FAILED(hr)) {
      return hr;
    }

    vector.reserve(upperBound-lowerBound);

    T* values;
    SafeArrayAccessData(pArray, reinterpret_cast<void**> (&values));
    for (; lowerBound <= upperBound; lowerBound++) {
      T v = values[lowerBound];
      vector.push_back(v);
    }
    SafeArrayUnaccessData(pArray);

    return S_OK;
  }
}

#endif /* MZQT_COMINTERFACE_HPP_ */
