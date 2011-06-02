#include "mbed.h"
#include "EthernetNetIf.h"
#include "MySQLClient.h"

#define SQL_SERVER   "sqlserver"
#define SQL_USER     "user"
#define SQL_PASSWORD "password"
#define SQL_DB       "dbname"

EthernetNetIf eth; 
MySQLClient sql;

MySQLResult sqlLastResult;
void onMySQLResult(MySQLResult r)
{
  sqlLastResult = r;
}

int main()
{
  printf("Start\n");

  printf("Setting up...\n");
  EthernetErr ethErr = eth.setup();
  if(ethErr)
  {
    printf("Error %d in setup.\n", ethErr);
    return -1;
  }
  printf("Setup OK\n");
  
  Host host(IpAddr(), 3306, SQL_SERVER);
  
  //Connect
  sqlLastResult = sql.open(host, SQL_USER, SQL_PASSWORD, SQL_DB, onMySQLResult);
  while(sqlLastResult == MYSQL_PROCESSING)
  {
    Net::poll();
  }
  if(sqlLastResult != MYSQL_OK)
  {
    printf("Error %d during connection\n", sqlLastResult);
  }else{
    printf("SQL connection OK\n");
  }
  
  //SQL Command
  //Make command
  char cmd[256] = {0};
  const char* msg="HOHO";
  sprintf(cmd, "INSERT INTO `base`.`table` (`col`) VALUES ('%s');", msg);
  
  //INSERT INTO DB
  string cmdStr = string(cmd); 
  sqlLastResult = sql.sql(cmdStr);
  while(sqlLastResult == MYSQL_PROCESSING)
  {
    Net::poll();
  }
  if(sqlLastResult != MYSQL_OK)
  {
    printf("Error %d during SQL Command\n", sqlLastResult);
  }
  
  sql.exit();
  
  while(1)
  {
  
  }
  
  return 0;
}
