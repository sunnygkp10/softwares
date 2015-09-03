<?hh
namespace APIs\Infrastructure\Objects;
use APIs\Domain\Https\HttpQuery;
use APIs\Domain\Https\URLs\URL;

final class ConcreteHttpQuery implements HttpQuery {
    private $url;
    private $content;
    private $httpCode;
    private static $validHttpCodes = Map<integer, string> {
        100 => "Continue",
        102 => "Processing",

        200 => "OK",
        201 => "Created",
        202 => "Accepted",
        203 => "Non-Authoritative Information",
        204 => "No Content",
        205 => "Reset Content",
        206 => "Partial Content",
        207 => "Multi-Status",

        300 => "Multiple Choices",
        301 => "Moved Permanently",
        302 => "Found",
        303 => "See Other",
        304 => "Not Modified",
        305 => "Use Proxy",
        307 => "Temporary Redirect",

        400 => "Bad Request",
        401 => "Authorization Required",
        402 => "Payment Required",
        403 => "Forbidden",
        404 => "Not Found",
        405 => "Method Not Allowed",
        406 => "Not Acceptable",
        407 => "Proxy Authentication Required",
        408 => "Request Time-out",
        409 => "Conflict",
        410 => "Gone",
        411 => "Length Required",
        412 => "Precondition Failed",
        413 => "Request Entity Too Large",
        414 => "Request-URI Too Large",
        415 => "Unsupported Media Type",
        416 => "Requested Range Not Satisfiable",
        417 => "Expectation Failed",
        422 => "Unprocessable Entity",
        423 => "Locked",
        424 => "Failed Dependency",
        425 => "No code",
        426 => "Upgrade Required",

        500 => "Internal Server Error",
        501 => "Method Not Implemented",
        502 => "Bad Gateway",
        503 => "Service Temporarily Unavailable",
        504 => "Gateway Time-out",
        505 => "HTTP Version Not Supported",
        506 => "Variant Also Negotiates",
        507 => "Insufficient Storage",
        510 => "Not Extended"
    };
    public function __construct(URL $url, string $content, integer $httpCode) {

        if (!isset(self::$validHttpCodes[$httpCode])) {
            throw new \Exception('The given httpCode ('.$httpCode.') is invalid.');
        }

        $this->url = $url;
        $this->content = $content;
        $this->httpCode = $httpCode;
    }

    public function getUrl(): URL {
        return $this->url;
    }

    public function getContent(): string {
        return $this->content;
    }

    public function getHttpCode(): integer {
        return $this->httpCode;
    }

}
