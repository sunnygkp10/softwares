<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Domain\Configurations\Params\Param;
use APIs\Domain\Configurations\Logics\Logic;
use APIs\Infrastructure\Objects\ConcreteLogic;

final class ConcreteLogicTest extends \PHPUnit_Framework_TestCase {
    private $functionCodeMock;
    private $paramMock;
    private $logicMock;
    private $params;
    private $subLogics;
    private $emptyParams;
    private $emptyLogics;
    public function setUp() {

        $this->functionCodeMock = $this->getMock('APIs\Domain\Configurations\Functions\FunctionCode');
        $this->paramMock = $this->getMock('APIs\Domain\Configurations\Params\Param');
        $this->logicMock = $this->getMock('APIs\Domain\Configurations\Logics\Logic');

        $this->params = Map<string, ?Param> {
            'first' => $this->paramMock,
            'second' => null
        };

        $this->subLogics = Map<string, Logic> {
            'logic' => $this->logicMock
        };

        $this->emptyParams = Map<string, ?Param> {};
        $this->emptyLogics = Map<string, Logic> {};

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $logic = new ConcreteLogic($this->functionCodeMock);

        $this->assertEquals($this->functionCodeMock, $logic->getFunctionCode());
        $this->assertFalse($logic->hasParams());
        $this->assertNull($logic->getParams());
        $this->assertFalse($logic->hasSubLogics());
        $this->assertNull($logic->getSubLogics());

    }

    public function testCreate_withEmptyParams_withEmptyLogics_Success() {

        $logic = new ConcreteLogic($this->functionCodeMock, $this->emptyParams, $this->emptyLogics);

        $this->assertEquals($this->functionCodeMock, $logic->getFunctionCode());
        $this->assertFalse($logic->hasParams());
        $this->assertNull($logic->getParams());
        $this->assertFalse($logic->hasSubLogics());
        $this->assertNull($logic->getSubLogics());

    }

    public function testCreate_withParams_Success() {

        $logic = new ConcreteLogic($this->functionCodeMock, $this->params);

        $this->assertEquals($this->functionCodeMock, $logic->getFunctionCode());
        $this->assertTrue($logic->hasParams());
        $this->assertEquals($this->params, $logic->getParams());
        $this->assertFalse($logic->hasSubLogics());
        $this->assertNull($logic->getSubLogics());

    }

    public function testCreate_withSubLogics_Success() {

        $logic = new ConcreteLogic($this->functionCodeMock, null, $this->subLogics);

        $this->assertEquals($this->functionCodeMock, $logic->getFunctionCode());
        $this->assertFalse($logic->hasParams());
        $this->assertNull($logic->getParams());
        $this->assertTrue($logic->hasSubLogics());
        $this->assertEquals($this->subLogics, $logic->getSubLogics());

    }

    public function testCreate_withParams_withSubLogics_Success() {

        $logic = new ConcreteLogic($this->functionCodeMock, $this->params, $this->subLogics);

        $this->assertEquals($this->functionCodeMock, $logic->getFunctionCode());
        $this->assertTrue($logic->hasParams());
        $this->assertEquals($this->params, $logic->getParams());
        $this->assertTrue($logic->hasSubLogics());
        $this->assertEquals($this->subLogics, $logic->getSubLogics());

    }

}
