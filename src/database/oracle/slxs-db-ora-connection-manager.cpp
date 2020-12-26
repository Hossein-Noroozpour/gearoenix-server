#include "slxs-db-ora-connection-manager.hpp"
#include "../../core/slxs-core-configurations.hpp"
#include "../../log/slxs-log.hpp"
#include <oci.h>

static void checkerr(OCIError *errhp, sword status)
{
text errbuf[512];
sb4 errcode = 0;

switch (status)
{
case OCI_SUCCESS:
break;
case OCI_SUCCESS_WITH_INFO:
(void) printf("Error - OCI_SUCCESS_WITH_INFO\n");
break;
case OCI_NEED_DATA:
(void) printf("Error - OCI_NEED_DATA\n");
break;
case OCI_NO_DATA:
(void) printf("Error - OCI_NODATA\n");
break;
case OCI_ERROR:
(void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
(void) printf("Error - %.*s\n", 512, errbuf);
break;
case OCI_INVALID_HANDLE:
(void) printf("Error - OCI_INVALID_HANDLE\n");
break;
case OCI_STILL_EXECUTING:
(void) printf("Error - OCI_STILL_EXECUTE\n");
break;
case OCI_CONTINUE:
(void) printf("Error - OCI_CONTINUE\n");
break;
default:
break;
}
}

slxs::database::oracle::ConnectionManager::ConnectionManager() noexcept {
    const auto& con_str = core::Configurations::get_connection_string();
    const auto threads_count = core::Configurations::get_threads_count();
    constexpr const char USER_DEL [] = ";User ID=";
    std::string tmp = con_str.substr(con_str.find(USER_DEL)).substr(sizeof(USER_DEL) - 1);
    const auto user = tmp.substr(0, tmp.find(';')) + "\0";
    constexpr const char PASS_DEL [] = ";Password=";
    tmp = con_str.substr(con_str.find(PASS_DEL)).substr(sizeof(PASS_DEL) - 1);
    const auto pass = tmp.substr(0, tmp.find(';')) + "\0";
    LOG_D("Oracle username and password is (" << user << ", " << pass << ")")

    auto error_code = OCIEnvCreate(&environment, OCI_DEFAULT, nullptr, nullptr, nullptr, nullptr, 0, nullptr);
    if(error_code != 0) {
        LOG_F("Oracle environment creation failed with code: " << error_code)
    }

    const std::string constr("10.1.1.182:1521/stgdb");

    (void) OCIHandleAlloc(environment, reinterpret_cast<dvoid **>(&error), OCI_HTYPE_ERROR,0, nullptr);
    (void) OCIHandleAlloc(environment, reinterpret_cast<dvoid **>(&server), OCI_HTYPE_SERVER,0, nullptr);
    (void) OCIHandleAlloc(environment, reinterpret_cast<dvoid **>(&service_context), OCI_HTYPE_SVCCTX, 0, nullptr);
    error_code = OCIServerAttach(server, error, (text *) constr.c_str(), (sb4) constr.size(), 0);
    checkerr(error, error_code);
    if(OCI_SUCCESS != error_code)
        LOG_F("Session creation failed.")
    /* set attribute server context in the service context */
    (void) OCIAttrSet(service_context, OCI_HTYPE_SVCCTX, server, 0, OCI_ATTR_SERVER, error);
    (void) OCIHandleAlloc(environment, (dvoid **)&session, OCI_HTYPE_SESSION, 0, nullptr);
    (void) OCIAttrSet(session, OCI_HTYPE_SESSION, (dvoid *) user.c_str(), (ub4) user.size(), OCI_ATTR_USERNAME, error);
    (void) OCIAttrSet(session, OCI_HTYPE_SESSION, (dvoid *) pass.c_str(), (ub4) pass.size(), OCI_ATTR_PASSWORD, error);
    if(OCI_SUCCESS != OCISessionBegin (service_context, error, session, OCI_CRED_RDBMS, OCI_DEFAULT))
        LOG_F("Session begin failed.")
    (void) OCIAttrSet(service_context, OCI_HTYPE_SVCCTX, session, 0, OCI_ATTR_SESSION, error);

    auto logged_on = TRUE;

    OCIStmt *statement = nullptr;
    checkerr(error, OCIHandleAlloc(environment, (dvoid **) &statement, OCI_HTYPE_STMT, 0, nullptr));
    /* Retrieve the current maximum employee number. */
    const std::string que_str("SELECT cc01_id FROM stl.tc01_letter_template WHERE cc01_id = 161");
    checkerr(error, OCIStmtPrepare(statement, error, (text *) que_str.c_str(), (ub4) que_str.size(), OCI_NTV_SYNTAX, OCI_DEFAULT));
    OCIDefine *defnp = nullptr;
    sword    empno = 99;
    checkerr(error, OCIDefineByPos(statement, &defnp, error, 1, (dvoid *) &empno,
                                   (sword) sizeof(sword), SQLT_INT, (dvoid *) 0, (ub2 *)0,
                                   (ub2 *)0, OCI_DEFAULT));
    /* execute and fetch */
    checkerr(error, OCIStmtExecute(service_context, statement, error, (ub4) 1, (ub4) 0,
                                (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT));
    LOG_D("empno " << empno)
}

slxs::database::oracle::ConnectionManager::~ConnectionManager() noexcept {
}
