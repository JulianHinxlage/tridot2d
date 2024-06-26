//
// Copyright (c) 2021 Julian Hinxlage. All rights reserved.
//

#include "FrameBuffer.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <algorithm>

namespace tridot2d {

    FrameBuffer::FrameBuffer() {
        id = 0;
        width = 0;
        height = 0;
    }

    FrameBuffer::FrameBuffer(const FrameBuffer &frameBuffer) {
        id = 0;
        width = frameBuffer.width;
        height = frameBuffer.height;
        for(auto &attachment : frameBuffer.attachments){
            setAttachment(attachment.second.spec);
        }
    }

    FrameBuffer::FrameBuffer(uint32_t width, uint32_t height, std::vector<FrameBufferAttachmentSpec> specs) {
        id = 0;
        this->width = 0;
        this->height = 0;
        init(width, height, specs);
    }

    FrameBuffer::~FrameBuffer() {
        if(id != 0){
            glDeleteFramebuffers(1, &id);
            id = 0;
        }
    }

    static uint32_t currentFrameBufferId = 0;

    void bindFrameBuffer(uint32_t id){
        if(currentFrameBufferId != id){
            glBindFramebuffer(GL_FRAMEBUFFER, id);
            currentFrameBufferId = id;
        }
    }

    void FrameBuffer::bind() const {
        if(currentFrameBufferId != id) {
            bindFrameBuffer(id);
            glViewport(0, 0, width, height);

            std::vector<GLenum> drawBuffers;
            for(auto &attachment : attachments){
                if(attachment.second.texture.get() != nullptr) {
                    if (attachment.first >= (int)TextureAttachment::COLOR && attachment.first < (int)TextureAttachment::COLOR + 16) {
                        if(attachment.second.texture->getWidth() != width || attachment.second.texture->getHeight() != height){
                            glViewport(0, 0, attachment.second.texture->getWidth(), attachment.second.texture->getHeight());
                        }
                        drawBuffers.push_back(internalEnum((TextureAttachment)attachment.first));
                    }
                }
            }
            std::sort(drawBuffers.begin(), drawBuffers.end());
            glDrawBuffers(drawBuffers.size(), drawBuffers.data());
        }
    }

    void FrameBuffer::unbind() {
        if(currentFrameBufferId != 0) {
            bindFrameBuffer(0);
            GLFWwindow* window = glfwGetCurrentContext();
            if (window) {
                int width = 0;
                int height = 0;
                glfwGetWindowSize(window, &width, &height);
                if (width != 0 && height != 0) {
                    glViewport(0, 0, width, height);
                }
            }
        }
    }

    uint32_t FrameBuffer::getId() const {
        return id;
    }

    glm::vec2 FrameBuffer::getSize() {
        return glm::vec2(width, height);
    }

    void FrameBuffer::init(uint32_t width, uint32_t height, std::vector<FrameBufferAttachmentSpec> specs) {
        this->width = width;
        this->height = height;
        for(auto &spec : specs){
            setAttachment(spec);
        }
    }

    std::shared_ptr<Texture> FrameBuffer::getAttachment(TextureAttachment attachment) {
        auto entry = attachments.find((uint32_t)attachment);
        if(entry != attachments.end()){
            return entry->second.texture;
        }else{
            return nullptr;
        }
    }

    std::shared_ptr<Texture> FrameBuffer::getAttachment(const std::string& name) {
        for (auto& attachment : attachments) {
            if (attachment.second.texture.get() != nullptr) {
                if (attachment.second.spec.name == name) {
                    return attachment.second.texture;
                }
            }
        }
        return nullptr;
    }

    const FrameBufferAttachmentSpec* FrameBuffer::getAttachmentSpec(TextureAttachment attachment) {
        auto entry = attachments.find((uint32_t)attachment);
        if (entry != attachments.end()) {
            return &entry->second.spec;
        }
        else {
            return nullptr;
        }
    }

    const FrameBufferAttachmentSpec* FrameBuffer::getAttachmentSpec(const std::string& name) {
        for (auto& attachment : attachments) {
            if (attachment.second.texture.get() != nullptr) {
                if (attachment.second.spec.name == name) {
                    return &attachment.second.spec;
                }
            }
        }
        return nullptr;
    }

    std::shared_ptr<Texture> FrameBuffer::setAttachment(FrameBufferAttachmentSpec spec, const std::shared_ptr<Texture> &texture) {
        if(id == 0){
            glGenFramebuffers(1, &id);
        }
        bindFrameBuffer(id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, internalEnum(spec.type), GL_TEXTURE_2D, texture->getId(), 0);

        auto entry = attachments.find((uint32_t)spec.type);
        if(entry != attachments.end()){
            texture->setMagMin(spec.magNearest, spec.minNearest);
            texture->setWrap(spec.sRepeat, spec.tRepeat);
            if (spec.useBorderColor) {
                texture->setBorderColor(spec.borderColor);
            }
            entry->second.spec = spec;
            entry->second.texture = texture;
            return entry->second.texture;
        }else{
            texture->setMagMin(spec.magNearest, spec.minNearest);
            texture->setWrap(spec.sRepeat, spec.tRepeat);
            if (spec.useBorderColor) {
                texture->setBorderColor(spec.borderColor);
            }
            attachments[(uint32_t)spec.type] = {texture, spec};
            return attachments[(uint32_t)spec.type].texture;
        }
    }

    std::shared_ptr<Texture> FrameBuffer::setAttachment(FrameBufferAttachmentSpec spec) {
        auto entry = attachments.find((uint32_t)spec.type);
        if(entry != attachments.end()){
            entry->second.spec = spec;
            return entry->second.texture;
        }

        std::shared_ptr<Texture> texture;
        switch (spec.type) {
            case TextureAttachment::DEPTH: {
                texture = std::make_shared<Texture>();
                TextureFormat format = TextureFormat::DEPTH24;
                if (spec.textureFormat != TextureFormat::TEXTURE_FORMAT_NONE) {
                    format = spec.textureFormat;
                }
                texture->create(width, height, format, spec.mipMapping);
                break;
            }
            case TextureAttachment::STENCIL: {
                texture = std::make_shared<Texture>();
                TextureFormat format = TextureFormat::DEPTH24STENCIL8;
                if (spec.textureFormat != TextureFormat::TEXTURE_FORMAT_NONE) {
                    format = spec.textureFormat;
                }
                texture->create(width, height, format, spec.mipMapping);
                break;
            }
            default: {
                if(spec.type >= TextureAttachment::COLOR && (int)spec.type < (int)TextureAttachment::COLOR + 16){
                    texture = std::make_shared<Texture>();
                    TextureFormat format = TextureFormat::RGBA8;
                    if (spec.textureFormat != TextureFormat::TEXTURE_FORMAT_NONE) {
                        format = spec.textureFormat;
                    }
                    texture->create(width, height, format, spec.mipMapping);
                }
                break;
            }
        }
        return setAttachment(spec, texture);
    }

    void FrameBuffer::resize(uint32_t width, uint32_t height) {
        bindFrameBuffer(id);
        for(auto &attachment : attachments){
            auto &texture = attachment.second.texture;
            if(texture.get() != nullptr){
                if(attachment.second.spec.resize){
                    int x = width * attachment.second.spec.resizeFactor.x;
                    int y = height * attachment.second.spec.resizeFactor.y;
                    if(texture->getWidth() != x || texture->getHeight() != y){
                        texture->create(x, y, texture->getFormat(), false);
                        glFramebufferTexture2D(GL_FRAMEBUFFER, internalEnum((TextureAttachment)attachment.first), GL_TEXTURE_2D, texture->getId(), 0);
                    }
                }
            }
        }
        this->width = width;
        this->height = height;
        unbind();
        bind();
    }

    void FrameBuffer::clear() {
        uint32_t current = currentFrameBufferId;
        bindFrameBuffer(id);
        for(auto &attachment : attachments){
            clear((TextureAttachment)attachment.first);
        }
        bindFrameBuffer(current);
    }

    void FrameBuffer::clear(TextureAttachment attachment) {
        auto entry = attachments.find((uint32_t)attachment);
        if(entry != attachments.end()){
            if(entry->second.spec.clear){
                int bit = 0;
                if (entry->first == (int)TextureAttachment::DEPTH) {
                    bit |= GL_DEPTH_BUFFER_BIT;
                }else if (entry->first == (int)TextureAttachment::STENCIL) {
                    bit |= GL_STENCIL_BUFFER_BIT;
                }else if (entry->first >= (int)TextureAttachment::COLOR && entry->first < (int)TextureAttachment::COLOR + 16) {
                    bit |= GL_COLOR_BUFFER_BIT;
                }

                glm::vec4 color = entry->second.spec.clearColor.vec();
                glDrawBuffer(internalEnum((TextureAttachment)entry->first));
                glClearColor(color.r, color.g, color.b, color.a);
                glClear(bit);
            }
        }
    }

}