<?hh

function PDOConnect(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null) {

    $createDSN = function(string $driver, string $server, string $database = null) {

        if (empty($database)) {
            return $driver.':host='.$server.';';
        }

        return $driver.':dbname='.$database.";host=".$server;

    };

    if (!isset($params['username'])) {
        throw new \Exception('The username is mandatory in order to connect to the database.', 500);
    }

    if (!isset($params['password'])) {
        throw new \Exception('The password is mandatory in order to connect to the database.', 500);
    }

    if (!isset($params['server'])) {
        throw new \Exception('The server is mandatory in order to connect to the database.', 500);
    }

    if (!isset($params['driver'])) {
        throw new \Exception('The driver is mandatory in order to connect to the database.', 500);
    }

    try {
        $database = (isset($params['database']) ? $params['database'] : null);
        $dsn = $createDSN($params['driver'], $params['server'], $database);

        $pdo = new \PDO($dsn, $params['username'], $params['password']);
        return $pdo;

    } catch (\PDOException $exception) {
        throw new \Exception($exception->getMessage(), 500);
    }

};
