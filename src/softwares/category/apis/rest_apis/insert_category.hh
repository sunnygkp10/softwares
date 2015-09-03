<?hh
function insertCategory(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    $insertParams = Map<string, mixed> {
        'software_name' => $params['software_name'],
        'container_name' => $params['container_name'],
        'data' => $params['data']
    };

    $subLogics['insert_element']($insertParams);

}
