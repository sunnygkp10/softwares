<?hh
function retrieveCategory(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    $retrieveParams = Map<string, mixed> {
        'software_name' => $params['software_name'],
        'container_name' => $params['container_name'],
        'field_name' => 'slug',
        'field_value' => $params['slug']
    };

    $output = $subLogics['retrieve']($retrieveParams);
    return $output['content'];

}
