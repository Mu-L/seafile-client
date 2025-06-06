#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QUrl>
#include <QString>
#include <QMetaType>

#include "api/server-info.h"

class AccountInfo {
public:
    QString email;
    QString name;
    QString contact_email;
    qint64 totalStorage;
    qint64 usedStorage;
};

class Account {
    friend class AccountManager;
public:
    ServerInfo serverInfo;
    AccountInfo accountInfo;
    QUrl serverUrl;
    QString username;
    QString token;
    qint64 lastVisited;
    bool isShibboleth;
    bool isAutomaticLogin;
    QString s2fa_token;

    Account() : serverInfo(),
                lastVisited(0),
                isShibboleth(false),
                isAutomaticLogin(true) {}
    Account(QUrl serverUrl, QString username, QString token,
            qint64 lastVisited=0, bool isShibboleth = false,
            bool isAutomaticLogin = true, QString s2fa_token = QString())
        : serverInfo(),
          accountInfo(),
          serverUrl(serverUrl),
          username(username),
          token(token),
          lastVisited(lastVisited),
          isShibboleth(isShibboleth),
          isAutomaticLogin(isAutomaticLogin),
          s2fa_token(s2fa_token) {}

    Account(const Account &rhs)
      : serverInfo(rhs.serverInfo),
        accountInfo(rhs.accountInfo),
        serverUrl(rhs.serverUrl),
        username(rhs.username),
        token(rhs.token),
        lastVisited(rhs.lastVisited),
        isShibboleth(rhs.isShibboleth),
        isAutomaticLogin(rhs.isAutomaticLogin),
        s2fa_token(rhs.s2fa_token)
    {
    }

    Account& operator=(const Account&rhs) {
        serverInfo = rhs.serverInfo;
        accountInfo = rhs.accountInfo;
        serverUrl = rhs.serverUrl;
        username = rhs.username;
        token = rhs.token;
        lastVisited = rhs.lastVisited;
        isShibboleth = rhs.isShibboleth;
        isAutomaticLogin = rhs.isAutomaticLogin;
        s2fa_token = rhs.s2fa_token;
        return *this;
    }

    bool operator==(const Account& rhs) const {
        return serverUrl == rhs.serverUrl
            && username == rhs.username;
    }

    bool operator!=(const Account& rhs) const {
        return !(*this == rhs);
    }

    bool isValid() const {
        return token.length() > 0;
    }

    bool hasS2FAToken() const {
        return s2fa_token.length() > 0;
    }

    bool isPro() const {
        return serverInfo.proEdition;
    }

    bool hasOfficePreview() const {
        return serverInfo.officePreview;
    }

    bool hasFileSearch() const {
        return serverInfo.fileSearch;
    }

    bool hasDisableSyncWithAnyFolder() const {
        return serverInfo.disableSyncWithAnyFolder;
    }

    bool isAtLeastVersion(unsigned majorVersion, unsigned minorVersion, unsigned patchVersion) const {
        return (serverInfo.majorVersion << 20) +
               (serverInfo.minorVersion << 10) +
               (serverInfo.patchVersion) >=
               (majorVersion << 20) + (minorVersion << 10) + (patchVersion);
    }

    int getEncryptedLibraryVersion() const {
        return serverInfo.encryptedLibraryVersion;
    }

    QString getEncryptedLibraryPwdHashAlgo() const {
        return serverInfo.pwdHashAlgo;
    }

    QString getEncryptedLibraryPwdHashParams() const {
        return serverInfo.pwdHashParams;
    }

    // require pro edtions and version at least at ...
    // excluding OSS Version
    bool isAtLeastProVersion(unsigned majorVersion, unsigned minorVersion, unsigned patchVersion) const {
        return isPro() && isAtLeastVersion(majorVersion, minorVersion, patchVersion);
    }

    // require oss edtions and version at least at ...
    // excluding Pro Version
    bool isAtLeastOSSVersion(unsigned majorVersion, unsigned minorVersion, unsigned patchVersion) const {
        return !isPro() && isAtLeastVersion(majorVersion, minorVersion, patchVersion);
    }

    qint32 getTotalStorage() const {
        return accountInfo.totalStorage;
    }

    qint32 getUsedStorage() const {
        return accountInfo.usedStorage;
    }

    // The accountInfo.email will be an internal email address if the contact_email exists.
    QString getDisplayEmail() const {
        return accountInfo.contact_email.isEmpty() ? accountInfo.email : accountInfo.contact_email;
    }

    QUrl getAbsoluteUrl(const QString& relativeUrl) const;
    QString getSignature() const;
};

Q_DECLARE_METATYPE(Account)

#endif // ACCOUNT_H
