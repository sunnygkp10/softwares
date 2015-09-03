<?hh
function deleteCategory(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    $deleteParams = Map<string, mixed> {
        'software_name' => $params['software_name'],
        'container_name' => $params['container_name'],
        'uuid' => $params['category_uuid']
    };

    $subLogics['delete_element']($deleteParams);

}
