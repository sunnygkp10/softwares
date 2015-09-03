<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteFunctionCode;

final class ConcreteFunctionCodeTest extends \PHPUnit_Framework_TestCase {

    private $languageDefinitionMock;
    private $name;
    private $code;
    private $invalidName;
    private $invalidCode;
    public function setUp() {

        $this->languageDefinitionMock = $this->getMock('APIs\Domain\Configurations\Functions\Languages\LanguageDefinition');

        $this->name = 'my-name';
        $this->code = 'lets say this is some code.';
        $this->invalidName = '';
        $this->invalidCode = '';

    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $functionCode = new ConcreteFunctionCode($this->name, $this->languageDefinitionMock, $this->code);

        $this->assertEquals($this->name, $functionCode->getName());
        $this->assertEquals($this->languageDefinitionMock, $functionCode->getLanguage());
        $this->assertEquals($this->code, $functionCode->getCode());

        $this->assertTrue($functionCode instanceof \APIs\Domain\Configurations\Functions\FunctionCode);

    }

    public function testCreate_withInvalidName_throwsException() {

        $asserted = false;
        try {

            new ConcreteFunctionCode($this->invalidName, $this->languageDefinitionMock, $this->code);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

    public function testCreate_withInvalidCode_throwsException() {

        $asserted = false;
        try {

            new ConcreteFunctionCode($this->name, $this->languageDefinitionMock, $this->invalidCode);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
