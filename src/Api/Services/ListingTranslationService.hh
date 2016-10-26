<?hh //strict
namespace Etsy\Api\Services;

use Etsy\Api\Client;
use Etsy\Logger\Logger;
use Plenty\Modules\Item\DataLayer\Models\ItemDescription;

class ListingTranslationService
{
    /**
     * Client $client
     */
    private Client $client;

    /**
     * Logger $logger
     */
    private Logger $logger;

    public function __construct(
        Client $client,
        Logger $logger,
    )
    {
        $this->client = $client;
        $this->logger = $logger;
    }

    /**
     * @param int $listingId
     * @param ItemDescription $description
     * @param string $language
     */
    public function createListingTranslation(int $listingId, ItemDescription $description, string $language):void
    {
        //TODO need to be adjusted as soon as the itemDescriptionList exists
        $response = null;
        $tags = explode(',', $description->keywords);

        $data = [
            'listing_id'    => $listingId,
            'language'      => $language,
            'title'         => $description->name1,
            'description'   => strip_tags($description->description),
        ];

        if(count($tags) > 0 && strlen($tags[0]) > 0)
        {
            $data = [
                'tags'      => $tags
            ];
        }

        $response = $this->client->call('createListingTranslation', [
            'listing_id' => $listingId,
            'language' => $language,
        ],
        $data);

        if(is_null($response) || (array_key_exists('exception', $response) && $response['exception'] === true))
        {
            $this->logger->log('Could not create listing translation for listing id ' . $listingId  . '. Reason: ...');
            // TODO  throw exception
        }
    }
}