<?hh

function createSoftware(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['retrieve'])) {
        throw new \Exception('The retrieve closure is mandatory in order to create a software (createSoftware).', 500);
    }

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute closure is mandatory in order to create a software (createSoftware).', 500);
    }

    if (!isset($params['software_name'])) {
        throw new \Exception('The software_name is mandatory in order to create a software (createSoftware).', 500);
    }

    //verify that the database doesnt exists.  If it does, throws an exception:
    $query = "show databases like :name;";
    $databases = $subLogics['retrieve'](
        Map<string, string> {
            'query' => $query,
            'params' => Map<string, string> {
                ':name' => $params['software_name']
            }
        }
    );

    if (!empty($database)) {
        throw new \Exception('The given software ('.$params['software_name'].') already exists.', 400);
    }
    
    //create the database name:
    $query = "create database ".$params['software_name']." character set utf8 collate utf8_general_ci;";
    $subLogics['execute'](
        Map<string, string> {
            'query' => $query
        }
    );

};
