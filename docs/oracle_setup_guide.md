# Oracle 11g ODBC Setup Guide for Qt6

## Prerequisites

1. Oracle 11g Server running and accessible
2. Oracle 11g Client installed on your Windows machine
3. Qt 6 with SQL modules

## Step-by-Step Setup

### 1. Install Oracle Client

- Download Oracle 11g Client (Administrator or Runtime)
- Install to default location (usually `C:\oracle\product\11.2.0\client_1`)
- Add Oracle bin directory to Windows PATH environment variable

### 2. Configure TNS Names

Create/edit `tnsnames.ora` file in `ORACLE_HOME\network\admin\`:

```
ORCL =
  (DESCRIPTION =
    (ADDRESS = (PROTOCOL = TCP)(HOST = your_server_ip)(PORT = 1521))
    (CONNECT_DATA =
      (SERVER = DEDICATED)
      (SERVICE_NAME = orcl)
    )
  )
```

Replace:

- `your_server_ip` with actual Oracle server IP address
- `orcl` with your actual Oracle service name
- `1521` with actual port if different

### 3. Test TNS Connection

Open Command Prompt and run:

```cmd
tnsping ORCL
```

Should return successful response.

### 4. Create ODBC Data Source

1. Open **Control Panel** → **Administrative Tools** → **ODBC Data Sources (64-bit)**
2. Click **System DSN** tab
3. Click **Add**
4. Select **Oracle in OraClient11g_home1**
5. Fill in:
   - **Data Source Name**: `ORCL_DSN`
   - **Description**: Oracle 11g Connection
   - **TNS Service Name**: `ORCL`
   - **User ID**: `TEST`
6. Click **Test Connection**
7. Enter password when prompted: `record25`
8. Should show "Connection successful"

### 5. Alternative Connection Methods

If DSN doesn't work, try direct connection string in your Qt code:

```cpp
// Replace DSN line with:
db.setDatabaseName("DRIVER={Oracle in OraClient11g_home1};DBQ=ORCL;UID=TEST;PWD=record25;");
```

### 6. Troubleshooting Common Issues

#### TNS-12154: Could not resolve connect identifier

- Check `tnsnames.ora` file exists and is correctly formatted
- Verify ORACLE_HOME environment variable is set
- Ensure TNS_ADMIN points to correct directory

#### ODBC Driver Not Found

- Reinstall Oracle Client with ODBC components
- Check if both 32-bit and 64-bit drivers match your application

#### Connection Timeout

- Verify Oracle server is running
- Check firewall settings allow port 1521
- Test with SQL\*Plus: `sqlplus TEST/record25@ORCL`

### 7. Environment Variables to Check

```
ORACLE_HOME=C:\oracle\product\11.2.0\client_1
TNS_ADMIN=%ORACLE_HOME%\network\admin
PATH=%ORACLE_HOME%\bin;...
```

## Final Notes

- Always test with SQL\*Plus first before Qt application
- Use System DSN instead of User DSN for service applications
- Keep Oracle Client and Server versions compatible
