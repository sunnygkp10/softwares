<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteLanguageDefinition;

final class ConcreteLanguageDefinitionTest extends \PHPUnit_Framework_TestCase {

    private $urlMock;
    private $name;
    private $port;
    private $invalidName;
    public function setUp() {

        $this->urlMock = $this->getMock('APIs\Domain\Https\URLs\URL');
        $this->name = 'my-name';
        $this->port = rand(1, 500);
        $this->invalidName = '';
    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $query = new ConcreteLanguageDefinition($this->name, $this->urlMock, $this->port);

        $this->assertEquals($this->name, $query->getName());
        $this->assertEquals($this->urlMock, $query->getHost());
        $this->assertEquals($this->port, $query->getPort());

        $this->assertTrue($query instanceof \APIs\Domain\Configurations\Functions\Languages\LanguageDefinition);
    }

    public function testCreate_withInvalidName_throwsException() {

        $asserted = false;
        try {

            new ConcreteLanguageDefinition($this->invalidName, $this->urlMock, $this->port);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
