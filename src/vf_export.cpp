#include "vf_export.h"

#include "pyhelper.h"
#include <PythonBinding.h>
#include <pysharedreference.h>

#pragma push_macro("slots")
#undef slots
#include "Python.h"
#pragma pop_macro("slots")

using namespace vfExport;

vf_export::vf_export(QObject *parent, int id) : QObject(parent),
   m_entity(VfCpp::VeinModuleEntity::Ptr(new VfCpp::VeinModuleEntity(id),&QObject::deleteLater)), m_isInitalized(false)
{
    //m_entity=new VfCpp::VeinModuleEntity(id);
    QObject::connect(m_entity.data(),&VfCpp::VeinModuleEntity::sigAttached,this,&vf_export::initOnce);
}

bool vf_export::initOnce()
{
    if(!m_isInitalized){
        m_isInitalized=true;
        m_entity->createComponent("EntityName","ExportModule",VfCpp::cVeinModuleComponent::Direction::out);
        m_status=m_entity->createComponent("Status",false,VfCpp::cVeinModuleComponent::Direction::out);
        m_entity->createRpc(this,"RPC_Convert", VfCpp::cVeinModuleRpc::Param({{"p_session", "QString"},{"p_inputPath", "QString"},{"p_outputPath", "QString"},{"p_engine", "QString"},{"p_filter" , "QString"},{"p_parameters", "QString"}}));
        py =  new zPyInt::PythonBinding();
        if(py->init("pythonconverter_pkg.CppInterface") == true){
            m_status=true;
        }
    }
    return true;
}

VfCpp::VeinModuleEntity* vf_export::getVeinEntity() const
{
    return m_entity.data();
}

void vf_export::setVeinEntity(VfCpp::VeinModuleEntity* value)
{
    m_entity = VfCpp::VeinModuleEntity::Ptr(value);
}

QVariant vf_export::RPC_Convert(QVariantMap p_params)
{
    zPyInt::PythonGuard guard;
    bool retVal = false;

    m_inputPath=p_params["p_inputPath"].toString();
    m_outputPath=p_params["p_outputPath"].toString();
    m_engine=p_params["p_engine"].toString();
    m_session=p_params["p_session"].toString();
    m_parameters=p_params["p_parameters"].toString();
    m_filter=p_params["p_filter"].toString();

    if(m_status == false){
        retVal=false;
    }
    else if(m_inputPath != "" && m_outputPath != "" && m_session != ""){
        py->callFunction("setInputPath",{PyUnicode_InternFromString(m_inputPath.toUtf8())});
        py->callFunction("setOutputPath",{PyUnicode_FromString(m_outputPath.toUtf8())});
        py->callFunction("setEngine",{PyUnicode_FromString(m_engine.toUtf8())});
        py->callFunction("setSession",{PyUnicode_FromString(m_session.toUtf8())});
        py->callFunction("setParams",{PyUnicode_FromString(m_parameters.toUtf8())});
        py->callFunction("setFilter",{PyUnicode_FromString(m_filter.toUtf8())});
        zPyInt::PySharedRef good =py->callFunction("checkInputFile",{});
        if(PyObject_IsTrue(good.data())){
            zPyInt::PySharedRef ret=py->callFunction("convert",{});
            if(PyObject_IsTrue(ret.data())){
                retVal=true;
            }
        }

    }

    return retVal;
}
