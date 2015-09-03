<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteParam;

final class ConcreteParamTest extends \PHPUnit_Framework_TestCase {
    private $variableMock;
    private $inputValidatorMock;
    private $value;
    public function setUp() {

        $this->variableMock = $this->getMock('APIs\Domain\Configurations\Variables\Variable');
        $this->inputValidatorMock = $this->getMock('APIs\Domain\Configurations\Validators\Input\InputValidator');

        $this->value = 'some value';

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $param = new ConcreteParam($this->value);

        $this->assertTrue($param->isValue());
        $this->assertEquals($this->value, $param->getValue());
        $this->assertFalse($param->isVariable());
        $this->assertNull($param->getVariable());
        $this->assertFalse($param->isInputValidator());
        $this->assertNull($param->getInputValidator());

        $this->assertTrue($param instanceof \APIs\Domain\Configurations\Params\Param);

    }

    public function testCreate_withVariable_Success() {

        $param = new ConcreteParam(null, $this->variableMock);

        $this->assertFalse($param->isValue());
        $this->assertNull($param->getValue());
        $this->assertTrue($param->isVariable());
        $this->assertEquals($this->variableMock, $param->getVariable());
        $this->assertFalse($param->isInputValidator());
        $this->assertNull($param->getInputValidator());

    }

    public function testCreate_withInputValidator_Success() {

        $param = new ConcreteParam(null, null, $this->inputValidatorMock);

        $this->assertFalse($param->isValue());
        $this->assertNull($param->getValue());
        $this->assertFalse($param->isVariable());
        $this->assertNull($param->getVariable());
        $this->assertTrue($param->isInputValidator());
        $this->assertEquals($this->inputValidatorMock, $param->getInputValidator());

    }

    public function testCreate_withEmptyValue_Success() {

        $param = new ConcreteParam('');

        $this->assertTrue($param->isValue());
        $this->assertEquals('', $param->getValue());
        $this->assertFalse($param->isVariable());
        $this->assertNull($param->getVariable());
        $this->assertFalse($param->isInputValidator());
        $this->assertNull($param->getInputValidator());

    }

    public function testCreate_isNull_throwsException() {

        $asserted = false;
        try {

            new ConcreteParam();

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_multipleParams_throwsException() {

        $asserted = false;
        try {

            new ConcreteParam($this->value, $this->variableMock, $this->inputValidatorMock);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
