#ifndef ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_
#define ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_

#if defined(ELAPACKETIO_LIBRARY) && defined(QT_DLL)
#define ELA_PACKETIO_EXPORT Q_DECL_EXPORT
#elif defined(QT_DLL) // 使用动态库
#define ELA_PACKETIO_EXPORT Q_DECL_IMPORT
#else
#define ELA_PACKETIO_EXPORT
#endif

#endif //ELAWIDGETTOOLS_ELAPACKETIO_ELAPACKETIO_EXPORT_H_
