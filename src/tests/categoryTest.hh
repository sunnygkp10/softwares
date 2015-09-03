<?hh
final class categoryTest extends \PHPUnit_Framework_TestCase {

    public function setUp() {

    }

    public function tearDown() {

    }

    private function generateUuid() {

        return sprintf( '%04x%04x-%04x-%04x-%04x-%04x%04x%04x',
            // 32 bits for "time_low"
            mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ),

            // 16 bits for "time_mid"
            mt_rand( 0, 0xffff ),

            // 16 bits for "time_hi_and_version",
            // four most significant bits holds version number 4
            mt_rand( 0, 0x0fff ) | 0x4000,

            // 16 bits, 8 bits for "clk_seq_hi_res",
            // 8 bits for "clk_seq_low",
            // two most significant bits holds zero and one for variant DCE1.1
            mt_rand( 0, 0x3fff ) | 0x8000,

            // 48 bits for "node"
            mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff ), mt_rand( 0, 0xffff )
        );
    }

    private function executeCurlRequest(string $host, string $httpMethod, string $uri, array $data = null) {

        include_once('src/softwares/datastore/apis/logics/curl/execute.hh');

        $params = Map<string, string> {
            'host' => $host,
            'port' => 80,
            'uri' => $uri,
            'http_method' => $httpMethod
        };

        if (!empty($data)) {
            $params['data'] = $data;
        }

        return execute($params);
    }

    private function execute(string $httpMethod, string $uri, array $data = null) {

        return $this->executeCurlRequest('http://apis-category.softwares.irestful.com', $httpMethod, $uri, $data);
    }

    private function saveDatastoreSoftware_Success() {

        $data = array(
            'config_url' => 'http://code.irestful.com/configs/softwares/apis/datastore.json'
        );

        $host = 'http://softwares.irestful.com';
        $uri = '/';

        $output = $this->executeCurlRequest($host, 'post', $uri, $data);

        $this->assertEquals($output['http_code'], 200);
    }

    private function saveCategorySoftware_Success() {

        $data = array(
            'config_url' => 'http://code.irestful.com/configs/softwares/apis/category.json'
        );

        $host = 'http://softwares.irestful.com';
        $uri = '/';

        $output = $this->executeCurlRequest($host, 'post', $uri, $data);

        $this->assertEquals($output['http_code'], 200);
    }

    private function insertSoftware_Success(string $name) {

        $host = 'http://apis-datastore.softwares.irestful.com';
        $data = array('software_name' => $name);
        $output = $this->executeCurlRequest($host, 'post', '/', $data);

        $this->assertEquals($output['http_code'], 200);
    }

    private function insertContainer_Success(string $software, string $container) {

        $host = 'http://apis-datastore.softwares.irestful.com';
        $uri = '/'.$software;
        $data = $this->getCategoryContainerData($container);

        $output = $this->executeCurlRequest($host, 'post', $uri, $data);

        $this->assertEquals($output['http_code'], 200);
    }

    private function deleteSoftware_Success(string $name) {

        $host = 'http://apis-datastore.softwares.irestful.com';
        $uri = '/'.$name;
        $output = $this->executeCurlRequest($host, 'delete', $uri);

        $this->assertEquals($output['http_code'], 200);
    }

    private function deleteContainer_Success(string $software, string $container) {

        $host = 'http://apis-datastore.softwares.irestful.com';
        $uri = '/'.$software.'/'.$container;
        $output = $this->executeCurlRequest($host, 'delete', $uri);

        $this->assertEquals($output['http_code'], 200);

    }

    private function getCategoryContainerData(string $container) {
        return array(
            'name' => $container,
            'fields' => array(
                'uuid' => array(
                    'is_primary_key' => true,
                    'type' => 'binary',
                    'length' => 16,
                    'default' => 'not null',
                    'validator' => array(
                        'language' => array(
                            'name' => 'hack',
                            'host' => 'http://hack.languages.irestful.com',
                            'port' => 80
                        ),
                        'function' => array(
                            'url' => 'http://code.irestful.com/softwares/datastore/apis/validators/uuid.hh',
                            'name' => 'uuid',
                            'language' => 'hack'
                        )
                    )
                ),
                'slug' => array(
                    'type' => 'varchar',
                    'length' => 255,
                    'is_unique' => true,
                    'is_index' => true,
                    'default' => 'not null',
                    'validator' => array(
                        'language' => array(
                            'name' => 'hack',
                            'host' => 'http://hack.languages.irestful.com',
                            'port' => 80
                        ),
                        'function' => array(
                            'url' => 'http://code.irestful.com/softwares/datastore/apis/validators/slug.hh',
                            'name' => 'slug',
                            'language' => 'hack'
                        )
                    )
                ),
                'link_anchor' => array(
                    'type' => 'varchar',
                    'length' => 255,
                    'default' => 'not null',
                    'validator' => array(
                        'language' => array(
                            'name' => 'hack',
                            'host' => 'http://hack.languages.irestful.com',
                            'port' => 80
                        ),
                        'function' => array(
                            'url' => 'http://code.irestful.com/softwares/category/apis/validators/link_anchor.hh',
                            'name' => 'linkAnchor',
                            'language' => 'hack'
                        )
                    )
                ),
                'title' => array(
                    'type' => 'varchar',
                    'length' => 255,
                    'default' => 'not null'
                ),
                'description' => array(
                    'type' => 'text',
                    'default' => 'null'
                ),
                'parent_category' => array(
                    'type' => 'binary',
                    'length' => 16,
                    'is_index' => true,
                    'default' => 'null',
                    'validator' => array(
                        'language' => array(
                            'name' => 'hack',
                            'host' => 'http://hack.languages.irestful.com',
                            'port' => 80
                        ),
                        'function' => array(
                            'url' => 'http://code.irestful.com/softwares/datastore/apis/validators/uuid.hh',
                            'name' => 'uuid',
                            'language' => 'hack'
                        )
                    )
                ),
                'created_on' => array(
                    'type' => 'bigint',
                    'length' => 11,
                    'default' => 'not null'
                ),
                'last_updated_on' => array(
                    'type' => 'bigint',
                    'length' => 11,
                    'default' => 'null'
                )
            )
        );
    }

    private function getCategoryData() {

        return array(
            'uuid' => $this->generateUuid(),
            'slug' => 'simple-slug',
            'link_anchor' => 'SimpleSlug',
            'title' => 'This is the title of the Simple Slug',
            'description' => ' Pellentesque elit massa, vehicula eget nisi sed, aliquam tempus magna. In quis blandit ipsum. Integer tempor eros sapien, laoreet facilisis est congue et. Ut vestibulum vehicula nisl vitae tincidunt. Sed aliquet arcu eu augue laoreet vulputate. Nullam tincidunt, purus et laoreet fringilla, felis turpis condimentum nulla, a finibus ipsum mauris at ex. Morbi consectetur sem tortor, ut tincidunt sem volutpat in. ',
            'parent_category' => null,
            'created_on' => time()
        );

    }

    private function insertCategory_Success(array $data) {

        $output = $this->execute('post', '/', $data);

        $this->assertEquals($output['http_code'], 200);
    }

    private function retrieveCategory_Success(string $slug, array $data) {

        $uri = '/'.$slug;
        $output = $this->execute('get', $uri);
        $retrievedCategory = json_decode($output['content'], true);

        $this->assertEquals($output['http_code'], 200);
        $this->assertEquals($retrievedCategory, $data);
    }

    private function deleteCategory_Success(string $uuid) {

        $uri = '/'.$uuid;
        $output = $this->execute('delete', $uri);

        $this->assertEquals($output['http_code'], 200);
    }

    public function testCategory_Success() {

        $software = 'apis_category';
        $container = 'category';
        $data = $this->getCategoryData();

        //save both our application config files:
        $this->saveDatastoreSoftware_Success();
        $this->saveCategorySoftware_Success();

        //create the software and container:
        $this->insertSoftware_Success($software);
        $this->insertContainer_Success($software, $container);

        //create a new category:
        $this->insertCategory_Success($data);
        $this->retrieveCategory_Success($data['slug'], $data);
        $this->deleteCategory_Success($data['uuid']);

        $this->deleteContainer_Success($software, $container);
        $this->deleteSoftware_Success($software);
    }

}
