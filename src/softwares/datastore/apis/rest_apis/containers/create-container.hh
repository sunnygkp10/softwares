<?hh

function createContainer(Map<string, string> $params = null, Map<string, \Closure> $subLogics = null, Map<string, string> $subAPIs = null) {

    if (!isset($subLogics['execute'])) {
        throw new \Exception('The execute closure is mandatory in order to create a container.', 500);
    }

    if (!isset($params['container_name'])) {
        throw new \Exception('The container_name param is mandatory in order to create a container.', 500);
    }

    if (!isset($params['fields'])) {
        throw new \Exception('The fields param is mandatory in order to create a container.', 500);
    }

    $params['fields'] = json_decode($params['fields'], true);
    if (empty($params['fields'])) {
        throw new \Exception('The fields must be valid json.', 500);
    }

    $getType = function(string $type) {

        $map = array(
            'varchar' => true,
            'text' => false,
            'int' => true,
            'bigint' => true,
            'blob' => false,
            'binary' => true,
            'char' => true
        );

        if (isset($map[$type])) {
            return array(
                'name' => $type,
                'can_have_length' => $map[$type]
            );
        }

        return null;

    };

    $getDefault = function(string $default = null) {

        if ($default == 'null') {
            return 'default null';
        }

        if (($default == 'not null') || (empty($default))) {
            return 'not null';
        }

        if (!is_numeric($default)) {
            $default = "'".$default."'";
        }

        return 'default '.$default;
    };

    $getTypeWithLength = function(array $field) {

        if (isset($field['type']['can_have_length']) && isset($field['length']) && $field['type']['can_have_length']) {
            return $field['type']['name'].'('.$field['length'].')';
        }

        return $field['type']['name'];
    };

    $lines = array();
    $foreignKeys = array();
    $indexFields = array();

    foreach($params['fields'] as $name => $oneData) {

        $field = array();
        if (isset($oneData['is_primary_key']) && $oneData['is_primary_key']) {
            $field['primary_key'] = 'primary key';
        }

        if (isset($oneData['foreign_key']) && !empty($oneData['foreign_key'])) {
            $foreignKeys[$name] = $oneData['foreign_key'];
        }

        if (isset($oneData['is_unique']) && $oneData['is_unique']) {
            $field['unique'] = 'unique';
        }

        if (isset($oneData['type']) && !empty($oneData['type'])) {
            $field['type'] = $getType($oneData['type']);
        }

        $comment = '';
        if (isset($oneData['validator'])) {
            $validator = array(
                'validator' => $oneData['validator']
            );

            $encoded = json_encode($validator);
            $comment = " COMMENT '".$encoded."'";
        }

        $default = (isset($oneData['default']) ? $oneData['default'] : null);
        $field['default'] = $getDefault($default);

        if (isset($oneData['length']) && $oneData['length']) {

            if (!is_numeric($oneData['length'])) {
                //throw
            }

            $field['length'] = (integer) $oneData['length'];
        }

        if (isset($field['unique']) && $field['primary_key']) {
            unset($field['unique']);
        }

        if (isset($oneData['is_index']) && $oneData['is_index']) {
            $indexFields[] = $name;
        }

        $default = isset($field['default']) ? ' '.$field['default'] : '';
        $primaryKey = isset($field['primary_key']) ? ' '.$field['primary_key'] : '';
        $unique = isset($field['unique']) ? ' '.$field['unique'] : '';
        $lines[] = $name.' '.$getTypeWithLength($field).$default.$primaryKey.$unique.$comment;

    }

    foreach($indexFields as $fieldName) {
        $lines[] = 'index index_'.$params['container_name'].'___'.$fieldName.' ('.$fieldName.')';
    }

    foreach($foreignKeys as $fieldName => $oneData) {

        if (!isset($oneData['referenced_table']) || empty($oneData['referenced_table'])) {
            throw new \Exception('The referenced_table index must be defined in the foreign_key data.');
        }

        if (!isset($oneData['referenced_field']) || empty($oneData['referenced_field'])) {
            throw new \Exception('The referenced_field index must be defined in the foreign_key data.');
        }

        $lines[] = 'constraint fk_'.$params['container_name'].'___'.$fieldName.' foreign key ('.$oneData['referenced_table'].') references '.$oneData['referenced_table'].' ('.$oneData['referenced_field'].')';

    }

    $query = 'create table '.$params['container_name'].' ('.implode(',', $lines).') Engine=InnoDb;';
    $subLogics['execute'](
        Map<string, string> {
            'query' => $query
        }
    );
};
