<?hh
namespace APIs\Tests\Tests\Unit\Objects;
use APIs\Infrastructure\Objects\ConcreteHttpQuery;

final class ConcreteHttpQueryTest extends \PHPUnit_Framework_TestCase {

    private $urlMock;
    private $content;
    private $httpCode;
    private $invalidHttpCode;
    public function setUp() {

        $this->urlMock = $this->getMock('APIs\Domain\Https\URLs\URL');
        $this->content = 'some-content';
        $this->httpCode = 200;
        $this->invalidHttpCode = rand() * 1000;
    }

    public function tearDown() {

    }

    public function testCreate_Success() {

        $query = new ConcreteHttpQuery($this->urlMock, $this->content, $this->httpCode);

        $this->assertEquals($this->urlMock, $query->getUrl());
        $this->assertEquals($this->content, $query->getContent());
        $this->assertEquals($this->httpCode, $query->getHttpCode());

        $this->assertTrue($query instanceof \APIs\Domain\Https\HttpQuery);
    }

    public function testCreate_withInvalidHttpCode_throwsException() {

        $asserted = false;
        try {

            new ConcreteHttpQuery($this->urlMock, $this->content, $this->invalidHttpCode);

        } catch (\Exception $exception) {
            $asserted = true;
        }

        $this->assertTrue($asserted);

    }

}
