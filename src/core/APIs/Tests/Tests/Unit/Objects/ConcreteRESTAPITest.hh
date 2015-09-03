<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Domain\Configurations\Params\Param;
use APIs\Infrastructure\Objects\ConcreteRESTAPI;

final class ConcreteRESTAPITest extends \PHPUnit_Framework_TestCase {
    private $functionCodeMock;
    private $paramMock;
    private $logicMock;
    private $viewMock;
    private $renderedParserMock;
    private $restAPIMock;
    private $params;
    private $subLogics;
    private $emptyParams;
    private $emptySubLogics;
    private $restAPI;
    public function setUp() {

        $this->functionCodeMock = $this->getMock('APIs\Domain\Configurations\Functions\FunctionCode');
        $this->paramMock = $this->getMock('APIs\Domain\Configurations\Params\Param');
        $this->logicMock = $this->getMock('APIs\Domain\Configurations\Logics\Logic');
        $this->viewMock = $this->getMock('APIs\Domain\Configurations\Views\View');
        $this->renderedParserMock = $this->getMock('APIs\Domain\Configurations\Rendered\RenderedParsers\RenderedParser');
        $this->restAPIMock = $this->getMock('APIs\Domain\Configurations\RESTAPIs\RESTAPI');

        $this->params = Map<string, ?Param> {
            'first' => $this->paramMock,
            'second' => null
        };

        $this->subLogics = Map<string, Logic> {
            'first' => $this->logicMock
        };

        $this->emptyParams = Map<string, ?Param> {

        };

        $this->emptySubLogics = Map<string, Logic> {

        };

        $this->restAPI = new ConcreteRESTAPI($this->functionCodeMock);

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

        $this->assertTrue($api instanceof \APIs\Domain\Configurations\RESTAPIs\RESTAPI);

    }

    public function testCreate_withEmptyParams_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, $this->emptyParams);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withEmptySubLogics_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, $this->emptySubLogics);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withParams_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, $this->params);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertTrue($api->hasParams());
        $this->assertEquals($this->params, $api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withSubLogics_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, $this->subLogics);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertTrue($api->hasSubLogics());
        $this->assertEquals($this->subLogics, $api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withView_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, $this->viewMock);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertTrue($api->hasView());
        $this->assertEquals($this->viewMock, $api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withRenderedParser_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, null, $this->renderedParserMock);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertTrue($api->hasRenderedParser());
        $this->assertEquals($this->renderedParserMock, $api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withSuccessAPI_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, null, null, $this->restAPI);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertTrue($api->hasSuccessAPI());
        $this->assertEquals($this->restAPI, $api->getSuccessAPI());
        $this->assertFalse($api->hasFailureAPI());
        $this->assertNull($api->getFailureAPI());

    }

    public function testCreate_withFailureAPI_Success() {

        $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, null, null, null, $this->restAPI);

        $this->assertEquals($this->functionCodeMock, $api->getFunctionCode());
        $this->assertFalse($api->hasParams());
        $this->assertNull($api->getParams());
        $this->assertFalse($api->hasSubLogics());
        $this->assertNull($api->getSubLogics());
        $this->assertFalse($api->hasView());
        $this->assertNull($api->getView());
        $this->assertFalse($api->hasRenderedParser());
        $this->assertNull($api->getRenderedParser());
        $this->assertFalse($api->hasSuccessAPI());
        $this->assertNull($api->getSuccessAPI());
        $this->assertTrue($api->hasFailureAPI());
        $this->assertEquals($this->restAPI, $api->getFailureAPI());

    }

    public function testCreate_withSuccessAPIWithView_withView_throwsException() {

        $successRestAPI = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, $this->viewMock);

        $asserted = false;
        try {

            $api = new ConcreteRESTAPI($this->functionCodeMock, null, null, null, $this->viewMock, null, $successRestAPI);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }



}
