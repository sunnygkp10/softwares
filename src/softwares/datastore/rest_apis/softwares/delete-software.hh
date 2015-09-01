<?hh

function deleteSoftware(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute subLogic is mandatory in order to delete a software.', 500);
    }

    if (!isset($subAPIs['retrieve_software'])) {
        throw new \Exception('The retrieve_software subAPI is mandatory in order to delete a software.', 500);
    }

    if (!isset($params['software_name'])) {
        throw new \Exception('The software_name param is mandatory in order to delete a software.', 500);
    }

    //retrieve our software:
    $software = $subAPIs['retrieve_software']();

    //if we have containers and if we dont' force the delete request, we throw an exception:
    $hasContainers = isset($software['containers']) && !empty($software['containers']);
    $isForced = (isset($params['forced']) ? ((boolean) $params['forced']): false);
    if ($hasContainers && !$isForced) {
        throw new \Exception('The software ('.$params['software_name'].') cannot be deleted because it contains containers.  To force delete the containers, you must add the "forced" parameter.', 400);
    }

    //we delete the software:
    $query = "drop database ".$params['software_name'];
    $subLogics['execute'](
        Map<string, string> {
            'query' => $query
        }
    );

};
