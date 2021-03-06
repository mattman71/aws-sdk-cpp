/*
* Copyright 2010-2016 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/
#include <aws/core/utils/Outcome.h>
#include <aws/core/auth/AWSAuthSigner.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/client/RetryStrategy.h>
#include <aws/core/http/HttpClient.h>
#include <aws/core/http/HttpResponse.h>
#include <aws/core/http/HttpClientFactory.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/threading/Executor.h>
#include <aws/glacier/GlacierClient.h>
#include <aws/glacier/GlacierEndpoint.h>
#include <aws/glacier/GlacierErrorMarshaller.h>
#include <aws/glacier/model/AbortMultipartUploadRequest.h>
#include <aws/glacier/model/AbortVaultLockRequest.h>
#include <aws/glacier/model/AddTagsToVaultRequest.h>
#include <aws/glacier/model/CompleteMultipartUploadRequest.h>
#include <aws/glacier/model/CompleteVaultLockRequest.h>
#include <aws/glacier/model/CreateVaultRequest.h>
#include <aws/glacier/model/DeleteArchiveRequest.h>
#include <aws/glacier/model/DeleteVaultRequest.h>
#include <aws/glacier/model/DeleteVaultAccessPolicyRequest.h>
#include <aws/glacier/model/DeleteVaultNotificationsRequest.h>
#include <aws/glacier/model/DescribeJobRequest.h>
#include <aws/glacier/model/DescribeVaultRequest.h>
#include <aws/glacier/model/GetDataRetrievalPolicyRequest.h>
#include <aws/glacier/model/GetJobOutputRequest.h>
#include <aws/glacier/model/GetVaultAccessPolicyRequest.h>
#include <aws/glacier/model/GetVaultLockRequest.h>
#include <aws/glacier/model/GetVaultNotificationsRequest.h>
#include <aws/glacier/model/InitiateJobRequest.h>
#include <aws/glacier/model/InitiateMultipartUploadRequest.h>
#include <aws/glacier/model/InitiateVaultLockRequest.h>
#include <aws/glacier/model/ListJobsRequest.h>
#include <aws/glacier/model/ListMultipartUploadsRequest.h>
#include <aws/glacier/model/ListPartsRequest.h>
#include <aws/glacier/model/ListTagsForVaultRequest.h>
#include <aws/glacier/model/ListVaultsRequest.h>
#include <aws/glacier/model/RemoveTagsFromVaultRequest.h>
#include <aws/glacier/model/SetDataRetrievalPolicyRequest.h>
#include <aws/glacier/model/SetVaultAccessPolicyRequest.h>
#include <aws/glacier/model/SetVaultNotificationsRequest.h>
#include <aws/glacier/model/UploadArchiveRequest.h>
#include <aws/glacier/model/UploadMultipartPartRequest.h>

using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Client;
using namespace Aws::Glacier;
using namespace Aws::Glacier::Model;
using namespace Aws::Http;
using namespace Aws::Utils::Json;

static const char* SERVICE_NAME = "glacier";
static const char* ALLOCATION_TAG = "GlacierClient";

GlacierClient::GlacierClient(const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(Aws::MakeShared<HttpClientFactory>(ALLOCATION_TAG), clientConfiguration,
    Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG, Aws::MakeShared<DefaultAWSCredentialsProviderChain>(ALLOCATION_TAG),
        SERVICE_NAME, clientConfiguration.authenticationRegion.empty() ? RegionMapper::GetRegionName(clientConfiguration.region) : clientConfiguration.authenticationRegion),
    Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_executor(clientConfiguration.executor)
{
  init(clientConfiguration);
}

GlacierClient::GlacierClient(const AWSCredentials& credentials, const Client::ClientConfiguration& clientConfiguration) :
  BASECLASS(Aws::MakeShared<HttpClientFactory>(ALLOCATION_TAG), clientConfiguration,
    Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG, Aws::MakeShared<SimpleAWSCredentialsProvider>(ALLOCATION_TAG, credentials),
         SERVICE_NAME, clientConfiguration.authenticationRegion.empty() ? RegionMapper::GetRegionName(clientConfiguration.region) : clientConfiguration.authenticationRegion),
    Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_executor(clientConfiguration.executor)
{
  init(clientConfiguration);
}

GlacierClient::GlacierClient(const std::shared_ptr<AWSCredentialsProvider>& credentialsProvider,
  const Client::ClientConfiguration& clientConfiguration, const std::shared_ptr<HttpClientFactory const>& httpClientFactory) :
  BASECLASS(httpClientFactory != nullptr ? httpClientFactory : Aws::MakeShared<HttpClientFactory>(ALLOCATION_TAG), clientConfiguration,
    Aws::MakeShared<AWSAuthV4Signer>(ALLOCATION_TAG, credentialsProvider,
         SERVICE_NAME, clientConfiguration.authenticationRegion.empty() ? RegionMapper::GetRegionName(clientConfiguration.region) : clientConfiguration.authenticationRegion),
    Aws::MakeShared<GlacierErrorMarshaller>(ALLOCATION_TAG)),
    m_executor(clientConfiguration.executor)
{
  init(clientConfiguration);
}

GlacierClient::~GlacierClient()
{
}

void GlacierClient::init(const ClientConfiguration& config)
{
  Aws::StringStream ss;
  ss << SchemeMapper::ToString(config.scheme) << "://";

  if(config.endpointOverride.empty() && config.authenticationRegion.empty())
  {
    ss << GlacierEndpoint::ForRegion(config.region);
  }
  else
  {
    ss << config.endpointOverride;
  }

  m_uri = ss.str();
}
AbortMultipartUploadOutcome GlacierClient::AbortMultipartUpload(const AbortMultipartUploadRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads/";
  ss << request.GetUploadId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return AbortMultipartUploadOutcome(NoResult());
  }
  else
  {
    return AbortMultipartUploadOutcome(outcome.GetError());
  }
}

AbortMultipartUploadOutcomeCallable GlacierClient::AbortMultipartUploadCallable(const AbortMultipartUploadRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::AbortMultipartUpload, this, request);
}

void GlacierClient::AbortMultipartUploadAsync(const AbortMultipartUploadRequest& request, const AbortMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::AbortMultipartUploadAsyncHelper, this, request, handler, context);
}

void GlacierClient::AbortMultipartUploadAsyncHelper(const AbortMultipartUploadRequest& request, const AbortMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, AbortMultipartUpload(request), context);
}

AbortVaultLockOutcome GlacierClient::AbortVaultLock(const AbortVaultLockRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/lock-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return AbortVaultLockOutcome(NoResult());
  }
  else
  {
    return AbortVaultLockOutcome(outcome.GetError());
  }
}

AbortVaultLockOutcomeCallable GlacierClient::AbortVaultLockCallable(const AbortVaultLockRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::AbortVaultLock, this, request);
}

void GlacierClient::AbortVaultLockAsync(const AbortVaultLockRequest& request, const AbortVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::AbortVaultLockAsyncHelper, this, request, handler, context);
}

void GlacierClient::AbortVaultLockAsyncHelper(const AbortVaultLockRequest& request, const AbortVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, AbortVaultLock(request), context);
}

AddTagsToVaultOutcome GlacierClient::AddTagsToVault(const AddTagsToVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/tags?operation=add";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return AddTagsToVaultOutcome(NoResult());
  }
  else
  {
    return AddTagsToVaultOutcome(outcome.GetError());
  }
}

AddTagsToVaultOutcomeCallable GlacierClient::AddTagsToVaultCallable(const AddTagsToVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::AddTagsToVault, this, request);
}

void GlacierClient::AddTagsToVaultAsync(const AddTagsToVaultRequest& request, const AddTagsToVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::AddTagsToVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::AddTagsToVaultAsyncHelper(const AddTagsToVaultRequest& request, const AddTagsToVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, AddTagsToVault(request), context);
}

CompleteMultipartUploadOutcome GlacierClient::CompleteMultipartUpload(const CompleteMultipartUploadRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads/";
  ss << request.GetUploadId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return CompleteMultipartUploadOutcome(CompleteMultipartUploadResult(outcome.GetResult()));
  }
  else
  {
    return CompleteMultipartUploadOutcome(outcome.GetError());
  }
}

CompleteMultipartUploadOutcomeCallable GlacierClient::CompleteMultipartUploadCallable(const CompleteMultipartUploadRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::CompleteMultipartUpload, this, request);
}

void GlacierClient::CompleteMultipartUploadAsync(const CompleteMultipartUploadRequest& request, const CompleteMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::CompleteMultipartUploadAsyncHelper, this, request, handler, context);
}

void GlacierClient::CompleteMultipartUploadAsyncHelper(const CompleteMultipartUploadRequest& request, const CompleteMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, CompleteMultipartUpload(request), context);
}

CompleteVaultLockOutcome GlacierClient::CompleteVaultLock(const CompleteVaultLockRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/lock-policy/";
  ss << request.GetLockId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return CompleteVaultLockOutcome(NoResult());
  }
  else
  {
    return CompleteVaultLockOutcome(outcome.GetError());
  }
}

CompleteVaultLockOutcomeCallable GlacierClient::CompleteVaultLockCallable(const CompleteVaultLockRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::CompleteVaultLock, this, request);
}

void GlacierClient::CompleteVaultLockAsync(const CompleteVaultLockRequest& request, const CompleteVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::CompleteVaultLockAsyncHelper, this, request, handler, context);
}

void GlacierClient::CompleteVaultLockAsyncHelper(const CompleteVaultLockRequest& request, const CompleteVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, CompleteVaultLock(request), context);
}

CreateVaultOutcome GlacierClient::CreateVault(const CreateVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_PUT);
  if(outcome.IsSuccess())
  {
    return CreateVaultOutcome(CreateVaultResult(outcome.GetResult()));
  }
  else
  {
    return CreateVaultOutcome(outcome.GetError());
  }
}

CreateVaultOutcomeCallable GlacierClient::CreateVaultCallable(const CreateVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::CreateVault, this, request);
}

void GlacierClient::CreateVaultAsync(const CreateVaultRequest& request, const CreateVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::CreateVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::CreateVaultAsyncHelper(const CreateVaultRequest& request, const CreateVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, CreateVault(request), context);
}

DeleteArchiveOutcome GlacierClient::DeleteArchive(const DeleteArchiveRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/archives/";
  ss << request.GetArchiveId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return DeleteArchiveOutcome(NoResult());
  }
  else
  {
    return DeleteArchiveOutcome(outcome.GetError());
  }
}

DeleteArchiveOutcomeCallable GlacierClient::DeleteArchiveCallable(const DeleteArchiveRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DeleteArchive, this, request);
}

void GlacierClient::DeleteArchiveAsync(const DeleteArchiveRequest& request, const DeleteArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DeleteArchiveAsyncHelper, this, request, handler, context);
}

void GlacierClient::DeleteArchiveAsyncHelper(const DeleteArchiveRequest& request, const DeleteArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DeleteArchive(request), context);
}

DeleteVaultOutcome GlacierClient::DeleteVault(const DeleteVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return DeleteVaultOutcome(NoResult());
  }
  else
  {
    return DeleteVaultOutcome(outcome.GetError());
  }
}

DeleteVaultOutcomeCallable GlacierClient::DeleteVaultCallable(const DeleteVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DeleteVault, this, request);
}

void GlacierClient::DeleteVaultAsync(const DeleteVaultRequest& request, const DeleteVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DeleteVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::DeleteVaultAsyncHelper(const DeleteVaultRequest& request, const DeleteVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DeleteVault(request), context);
}

DeleteVaultAccessPolicyOutcome GlacierClient::DeleteVaultAccessPolicy(const DeleteVaultAccessPolicyRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/access-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return DeleteVaultAccessPolicyOutcome(NoResult());
  }
  else
  {
    return DeleteVaultAccessPolicyOutcome(outcome.GetError());
  }
}

DeleteVaultAccessPolicyOutcomeCallable GlacierClient::DeleteVaultAccessPolicyCallable(const DeleteVaultAccessPolicyRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DeleteVaultAccessPolicy, this, request);
}

void GlacierClient::DeleteVaultAccessPolicyAsync(const DeleteVaultAccessPolicyRequest& request, const DeleteVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DeleteVaultAccessPolicyAsyncHelper, this, request, handler, context);
}

void GlacierClient::DeleteVaultAccessPolicyAsyncHelper(const DeleteVaultAccessPolicyRequest& request, const DeleteVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DeleteVaultAccessPolicy(request), context);
}

DeleteVaultNotificationsOutcome GlacierClient::DeleteVaultNotifications(const DeleteVaultNotificationsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/notification-configuration";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_DELETE);
  if(outcome.IsSuccess())
  {
    return DeleteVaultNotificationsOutcome(NoResult());
  }
  else
  {
    return DeleteVaultNotificationsOutcome(outcome.GetError());
  }
}

DeleteVaultNotificationsOutcomeCallable GlacierClient::DeleteVaultNotificationsCallable(const DeleteVaultNotificationsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DeleteVaultNotifications, this, request);
}

void GlacierClient::DeleteVaultNotificationsAsync(const DeleteVaultNotificationsRequest& request, const DeleteVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DeleteVaultNotificationsAsyncHelper, this, request, handler, context);
}

void GlacierClient::DeleteVaultNotificationsAsyncHelper(const DeleteVaultNotificationsRequest& request, const DeleteVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DeleteVaultNotifications(request), context);
}

DescribeJobOutcome GlacierClient::DescribeJob(const DescribeJobRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/jobs/";
  ss << request.GetJobId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return DescribeJobOutcome(DescribeJobResult(outcome.GetResult()));
  }
  else
  {
    return DescribeJobOutcome(outcome.GetError());
  }
}

DescribeJobOutcomeCallable GlacierClient::DescribeJobCallable(const DescribeJobRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DescribeJob, this, request);
}

void GlacierClient::DescribeJobAsync(const DescribeJobRequest& request, const DescribeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DescribeJobAsyncHelper, this, request, handler, context);
}

void GlacierClient::DescribeJobAsyncHelper(const DescribeJobRequest& request, const DescribeJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DescribeJob(request), context);
}

DescribeVaultOutcome GlacierClient::DescribeVault(const DescribeVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return DescribeVaultOutcome(DescribeVaultResult(outcome.GetResult()));
  }
  else
  {
    return DescribeVaultOutcome(outcome.GetError());
  }
}

DescribeVaultOutcomeCallable GlacierClient::DescribeVaultCallable(const DescribeVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::DescribeVault, this, request);
}

void GlacierClient::DescribeVaultAsync(const DescribeVaultRequest& request, const DescribeVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::DescribeVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::DescribeVaultAsyncHelper(const DescribeVaultRequest& request, const DescribeVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, DescribeVault(request), context);
}

GetDataRetrievalPolicyOutcome GlacierClient::GetDataRetrievalPolicy(const GetDataRetrievalPolicyRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/policies/data-retrieval";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return GetDataRetrievalPolicyOutcome(GetDataRetrievalPolicyResult(outcome.GetResult()));
  }
  else
  {
    return GetDataRetrievalPolicyOutcome(outcome.GetError());
  }
}

GetDataRetrievalPolicyOutcomeCallable GlacierClient::GetDataRetrievalPolicyCallable(const GetDataRetrievalPolicyRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::GetDataRetrievalPolicy, this, request);
}

void GlacierClient::GetDataRetrievalPolicyAsync(const GetDataRetrievalPolicyRequest& request, const GetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::GetDataRetrievalPolicyAsyncHelper, this, request, handler, context);
}

void GlacierClient::GetDataRetrievalPolicyAsyncHelper(const GetDataRetrievalPolicyRequest& request, const GetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, GetDataRetrievalPolicy(request), context);
}

GetJobOutputOutcome GlacierClient::GetJobOutput(const GetJobOutputRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/jobs/";
  ss << request.GetJobId();
  ss << "/output";

  StreamOutcome outcome = MakeRequestWithUnparsedResponse(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return GetJobOutputOutcome(GetJobOutputResult(outcome.GetResultWithOwnership()));
  }
  else
  {
    return GetJobOutputOutcome(outcome.GetError());
  }
}

GetJobOutputOutcomeCallable GlacierClient::GetJobOutputCallable(const GetJobOutputRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::GetJobOutput, this, request);
}

void GlacierClient::GetJobOutputAsync(const GetJobOutputRequest& request, const GetJobOutputResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::GetJobOutputAsyncHelper, this, request, handler, context);
}

void GlacierClient::GetJobOutputAsyncHelper(const GetJobOutputRequest& request, const GetJobOutputResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, GetJobOutput(request), context);
}

GetVaultAccessPolicyOutcome GlacierClient::GetVaultAccessPolicy(const GetVaultAccessPolicyRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/access-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return GetVaultAccessPolicyOutcome(GetVaultAccessPolicyResult(outcome.GetResult()));
  }
  else
  {
    return GetVaultAccessPolicyOutcome(outcome.GetError());
  }
}

GetVaultAccessPolicyOutcomeCallable GlacierClient::GetVaultAccessPolicyCallable(const GetVaultAccessPolicyRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::GetVaultAccessPolicy, this, request);
}

void GlacierClient::GetVaultAccessPolicyAsync(const GetVaultAccessPolicyRequest& request, const GetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::GetVaultAccessPolicyAsyncHelper, this, request, handler, context);
}

void GlacierClient::GetVaultAccessPolicyAsyncHelper(const GetVaultAccessPolicyRequest& request, const GetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, GetVaultAccessPolicy(request), context);
}

GetVaultLockOutcome GlacierClient::GetVaultLock(const GetVaultLockRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/lock-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return GetVaultLockOutcome(GetVaultLockResult(outcome.GetResult()));
  }
  else
  {
    return GetVaultLockOutcome(outcome.GetError());
  }
}

GetVaultLockOutcomeCallable GlacierClient::GetVaultLockCallable(const GetVaultLockRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::GetVaultLock, this, request);
}

void GlacierClient::GetVaultLockAsync(const GetVaultLockRequest& request, const GetVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::GetVaultLockAsyncHelper, this, request, handler, context);
}

void GlacierClient::GetVaultLockAsyncHelper(const GetVaultLockRequest& request, const GetVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, GetVaultLock(request), context);
}

GetVaultNotificationsOutcome GlacierClient::GetVaultNotifications(const GetVaultNotificationsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/notification-configuration";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return GetVaultNotificationsOutcome(GetVaultNotificationsResult(outcome.GetResult()));
  }
  else
  {
    return GetVaultNotificationsOutcome(outcome.GetError());
  }
}

GetVaultNotificationsOutcomeCallable GlacierClient::GetVaultNotificationsCallable(const GetVaultNotificationsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::GetVaultNotifications, this, request);
}

void GlacierClient::GetVaultNotificationsAsync(const GetVaultNotificationsRequest& request, const GetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::GetVaultNotificationsAsyncHelper, this, request, handler, context);
}

void GlacierClient::GetVaultNotificationsAsyncHelper(const GetVaultNotificationsRequest& request, const GetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, GetVaultNotifications(request), context);
}

InitiateJobOutcome GlacierClient::InitiateJob(const InitiateJobRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/jobs";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return InitiateJobOutcome(InitiateJobResult(outcome.GetResult()));
  }
  else
  {
    return InitiateJobOutcome(outcome.GetError());
  }
}

InitiateJobOutcomeCallable GlacierClient::InitiateJobCallable(const InitiateJobRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::InitiateJob, this, request);
}

void GlacierClient::InitiateJobAsync(const InitiateJobRequest& request, const InitiateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::InitiateJobAsyncHelper, this, request, handler, context);
}

void GlacierClient::InitiateJobAsyncHelper(const InitiateJobRequest& request, const InitiateJobResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, InitiateJob(request), context);
}

InitiateMultipartUploadOutcome GlacierClient::InitiateMultipartUpload(const InitiateMultipartUploadRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return InitiateMultipartUploadOutcome(InitiateMultipartUploadResult(outcome.GetResult()));
  }
  else
  {
    return InitiateMultipartUploadOutcome(outcome.GetError());
  }
}

InitiateMultipartUploadOutcomeCallable GlacierClient::InitiateMultipartUploadCallable(const InitiateMultipartUploadRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::InitiateMultipartUpload, this, request);
}

void GlacierClient::InitiateMultipartUploadAsync(const InitiateMultipartUploadRequest& request, const InitiateMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::InitiateMultipartUploadAsyncHelper, this, request, handler, context);
}

void GlacierClient::InitiateMultipartUploadAsyncHelper(const InitiateMultipartUploadRequest& request, const InitiateMultipartUploadResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, InitiateMultipartUpload(request), context);
}

InitiateVaultLockOutcome GlacierClient::InitiateVaultLock(const InitiateVaultLockRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/lock-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return InitiateVaultLockOutcome(InitiateVaultLockResult(outcome.GetResult()));
  }
  else
  {
    return InitiateVaultLockOutcome(outcome.GetError());
  }
}

InitiateVaultLockOutcomeCallable GlacierClient::InitiateVaultLockCallable(const InitiateVaultLockRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::InitiateVaultLock, this, request);
}

void GlacierClient::InitiateVaultLockAsync(const InitiateVaultLockRequest& request, const InitiateVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::InitiateVaultLockAsyncHelper, this, request, handler, context);
}

void GlacierClient::InitiateVaultLockAsyncHelper(const InitiateVaultLockRequest& request, const InitiateVaultLockResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, InitiateVaultLock(request), context);
}

ListJobsOutcome GlacierClient::ListJobs(const ListJobsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/jobs";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return ListJobsOutcome(ListJobsResult(outcome.GetResult()));
  }
  else
  {
    return ListJobsOutcome(outcome.GetError());
  }
}

ListJobsOutcomeCallable GlacierClient::ListJobsCallable(const ListJobsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::ListJobs, this, request);
}

void GlacierClient::ListJobsAsync(const ListJobsRequest& request, const ListJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::ListJobsAsyncHelper, this, request, handler, context);
}

void GlacierClient::ListJobsAsyncHelper(const ListJobsRequest& request, const ListJobsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, ListJobs(request), context);
}

ListMultipartUploadsOutcome GlacierClient::ListMultipartUploads(const ListMultipartUploadsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return ListMultipartUploadsOutcome(ListMultipartUploadsResult(outcome.GetResult()));
  }
  else
  {
    return ListMultipartUploadsOutcome(outcome.GetError());
  }
}

ListMultipartUploadsOutcomeCallable GlacierClient::ListMultipartUploadsCallable(const ListMultipartUploadsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::ListMultipartUploads, this, request);
}

void GlacierClient::ListMultipartUploadsAsync(const ListMultipartUploadsRequest& request, const ListMultipartUploadsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::ListMultipartUploadsAsyncHelper, this, request, handler, context);
}

void GlacierClient::ListMultipartUploadsAsyncHelper(const ListMultipartUploadsRequest& request, const ListMultipartUploadsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, ListMultipartUploads(request), context);
}

ListPartsOutcome GlacierClient::ListParts(const ListPartsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads/";
  ss << request.GetUploadId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return ListPartsOutcome(ListPartsResult(outcome.GetResult()));
  }
  else
  {
    return ListPartsOutcome(outcome.GetError());
  }
}

ListPartsOutcomeCallable GlacierClient::ListPartsCallable(const ListPartsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::ListParts, this, request);
}

void GlacierClient::ListPartsAsync(const ListPartsRequest& request, const ListPartsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::ListPartsAsyncHelper, this, request, handler, context);
}

void GlacierClient::ListPartsAsyncHelper(const ListPartsRequest& request, const ListPartsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, ListParts(request), context);
}

ListTagsForVaultOutcome GlacierClient::ListTagsForVault(const ListTagsForVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/tags";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return ListTagsForVaultOutcome(ListTagsForVaultResult(outcome.GetResult()));
  }
  else
  {
    return ListTagsForVaultOutcome(outcome.GetError());
  }
}

ListTagsForVaultOutcomeCallable GlacierClient::ListTagsForVaultCallable(const ListTagsForVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::ListTagsForVault, this, request);
}

void GlacierClient::ListTagsForVaultAsync(const ListTagsForVaultRequest& request, const ListTagsForVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::ListTagsForVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::ListTagsForVaultAsyncHelper(const ListTagsForVaultRequest& request, const ListTagsForVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, ListTagsForVault(request), context);
}

ListVaultsOutcome GlacierClient::ListVaults(const ListVaultsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_GET);
  if(outcome.IsSuccess())
  {
    return ListVaultsOutcome(ListVaultsResult(outcome.GetResult()));
  }
  else
  {
    return ListVaultsOutcome(outcome.GetError());
  }
}

ListVaultsOutcomeCallable GlacierClient::ListVaultsCallable(const ListVaultsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::ListVaults, this, request);
}

void GlacierClient::ListVaultsAsync(const ListVaultsRequest& request, const ListVaultsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::ListVaultsAsyncHelper, this, request, handler, context);
}

void GlacierClient::ListVaultsAsyncHelper(const ListVaultsRequest& request, const ListVaultsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, ListVaults(request), context);
}

RemoveTagsFromVaultOutcome GlacierClient::RemoveTagsFromVault(const RemoveTagsFromVaultRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/tags?operation=remove";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return RemoveTagsFromVaultOutcome(NoResult());
  }
  else
  {
    return RemoveTagsFromVaultOutcome(outcome.GetError());
  }
}

RemoveTagsFromVaultOutcomeCallable GlacierClient::RemoveTagsFromVaultCallable(const RemoveTagsFromVaultRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::RemoveTagsFromVault, this, request);
}

void GlacierClient::RemoveTagsFromVaultAsync(const RemoveTagsFromVaultRequest& request, const RemoveTagsFromVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::RemoveTagsFromVaultAsyncHelper, this, request, handler, context);
}

void GlacierClient::RemoveTagsFromVaultAsyncHelper(const RemoveTagsFromVaultRequest& request, const RemoveTagsFromVaultResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, RemoveTagsFromVault(request), context);
}

SetDataRetrievalPolicyOutcome GlacierClient::SetDataRetrievalPolicy(const SetDataRetrievalPolicyRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/policies/data-retrieval";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_PUT);
  if(outcome.IsSuccess())
  {
    return SetDataRetrievalPolicyOutcome(NoResult());
  }
  else
  {
    return SetDataRetrievalPolicyOutcome(outcome.GetError());
  }
}

SetDataRetrievalPolicyOutcomeCallable GlacierClient::SetDataRetrievalPolicyCallable(const SetDataRetrievalPolicyRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::SetDataRetrievalPolicy, this, request);
}

void GlacierClient::SetDataRetrievalPolicyAsync(const SetDataRetrievalPolicyRequest& request, const SetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::SetDataRetrievalPolicyAsyncHelper, this, request, handler, context);
}

void GlacierClient::SetDataRetrievalPolicyAsyncHelper(const SetDataRetrievalPolicyRequest& request, const SetDataRetrievalPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, SetDataRetrievalPolicy(request), context);
}

SetVaultAccessPolicyOutcome GlacierClient::SetVaultAccessPolicy(const SetVaultAccessPolicyRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/access-policy";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_PUT);
  if(outcome.IsSuccess())
  {
    return SetVaultAccessPolicyOutcome(NoResult());
  }
  else
  {
    return SetVaultAccessPolicyOutcome(outcome.GetError());
  }
}

SetVaultAccessPolicyOutcomeCallable GlacierClient::SetVaultAccessPolicyCallable(const SetVaultAccessPolicyRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::SetVaultAccessPolicy, this, request);
}

void GlacierClient::SetVaultAccessPolicyAsync(const SetVaultAccessPolicyRequest& request, const SetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::SetVaultAccessPolicyAsyncHelper, this, request, handler, context);
}

void GlacierClient::SetVaultAccessPolicyAsyncHelper(const SetVaultAccessPolicyRequest& request, const SetVaultAccessPolicyResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, SetVaultAccessPolicy(request), context);
}

SetVaultNotificationsOutcome GlacierClient::SetVaultNotifications(const SetVaultNotificationsRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/notification-configuration";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_PUT);
  if(outcome.IsSuccess())
  {
    return SetVaultNotificationsOutcome(NoResult());
  }
  else
  {
    return SetVaultNotificationsOutcome(outcome.GetError());
  }
}

SetVaultNotificationsOutcomeCallable GlacierClient::SetVaultNotificationsCallable(const SetVaultNotificationsRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::SetVaultNotifications, this, request);
}

void GlacierClient::SetVaultNotificationsAsync(const SetVaultNotificationsRequest& request, const SetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::SetVaultNotificationsAsyncHelper, this, request, handler, context);
}

void GlacierClient::SetVaultNotificationsAsyncHelper(const SetVaultNotificationsRequest& request, const SetVaultNotificationsResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, SetVaultNotifications(request), context);
}

UploadArchiveOutcome GlacierClient::UploadArchive(const UploadArchiveRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/archives";

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_POST);
  if(outcome.IsSuccess())
  {
    return UploadArchiveOutcome(UploadArchiveResult(outcome.GetResult()));
  }
  else
  {
    return UploadArchiveOutcome(outcome.GetError());
  }
}

UploadArchiveOutcomeCallable GlacierClient::UploadArchiveCallable(const UploadArchiveRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::UploadArchive, this, request);
}

void GlacierClient::UploadArchiveAsync(const UploadArchiveRequest& request, const UploadArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::UploadArchiveAsyncHelper, this, request, handler, context);
}

void GlacierClient::UploadArchiveAsyncHelper(const UploadArchiveRequest& request, const UploadArchiveResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, UploadArchive(request), context);
}

UploadMultipartPartOutcome GlacierClient::UploadMultipartPart(const UploadMultipartPartRequest& request) const
{
  Aws::StringStream ss;
  ss << m_uri << "/";
  ss << request.GetAccountId();
  ss << "/vaults/";
  ss << request.GetVaultName();
  ss << "/multipart-uploads/";
  ss << request.GetUploadId();

  JsonOutcome outcome = MakeRequest(ss.str(), request, HttpMethod::HTTP_PUT);
  if(outcome.IsSuccess())
  {
    return UploadMultipartPartOutcome(UploadMultipartPartResult(outcome.GetResult()));
  }
  else
  {
    return UploadMultipartPartOutcome(outcome.GetError());
  }
}

UploadMultipartPartOutcomeCallable GlacierClient::UploadMultipartPartCallable(const UploadMultipartPartRequest& request) const
{
  return std::async(std::launch::async, &GlacierClient::UploadMultipartPart, this, request);
}

void GlacierClient::UploadMultipartPartAsync(const UploadMultipartPartRequest& request, const UploadMultipartPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  m_executor->Submit(&GlacierClient::UploadMultipartPartAsyncHelper, this, request, handler, context);
}

void GlacierClient::UploadMultipartPartAsyncHelper(const UploadMultipartPartRequest& request, const UploadMultipartPartResponseReceivedHandler& handler, const std::shared_ptr<const Aws::Client::AsyncCallerContext>& context) const
{
  handler(this, request, UploadMultipartPart(request), context);
}

