<?hh

function deleteContainer(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute closure is mandatory in order to delete a container.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name is mandatory in order to delete a software.', 500);
    }

    //we delete the container:
    $query = "drop table ".$params['container_name'];
    $subLogics['execute'](
        Map<string, string> {
            'query' => $query
        }
    );
};
